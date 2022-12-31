using eCamServerV2.Data;
using eCamServerV2.Models;
using eCamServerV2.Utilities;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;

namespace eCamServerV2.Controllers
{
    [Route("api/manager")]
    [ApiController]
    public class MainManagerAPIController : ControllerBase
    {
        private readonly DbContextECam _context;
        private readonly NavigationAPIController _navigationApi;

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="context">Current application database context class, that inherits from <see cref="DbContext"/>.</param>
        public MainManagerAPIController(DbContextECam context, NavigationAPIController navigationApi)
        {
            _context = context;
            _navigationApi = navigationApi;
        }

        private async Task<List<WaypointModel>> WaypointsFromListAsync(List<int> waypointIds)
        {
            var result = new List<WaypointModel>();
            foreach(var waypointId in waypointIds)
            {
                var waypoint = await _context.Waypoints.FirstAsync(w => w.WaypointId == waypointId);
                result.Add(waypoint);
            }

            return result;
        }
        
        /// <summary>
        /// Creates a GOTO command from the root to the destination.
        /// </summary>
        /// <param name="root">Waypoint to travel from.</param>
        /// <param name="dest">Waypoint to travel to.</param>
        /// <returns>Command string.</returns>
        private string TaskCommand(WaypointModel root, WaypointModel dest)
        {
            if (root.Waypoint_ID_Top.HasValue)
            {
                if (root.Waypoint_ID_Top == dest.WaypointId)
                    return CommandConstructor.Goto(root.WaypointId, dest.QR_ID_Top, dest.QR_Distance_Top, "TOP");
            }
            if (root.Waypoint_ID_Bottom.HasValue)
            {
                if (root.Waypoint_ID_Bottom == dest.WaypointId)
                    return CommandConstructor.Goto(root.WaypointId, dest.QR_ID_Bottom, dest.QR_Distance_Bottom, "BOTTOM");
            }
            if (root.Waypoint_ID_Left.HasValue)
            {
                if (root.Waypoint_ID_Left == dest.WaypointId)
                    return CommandConstructor.Goto(root.WaypointId, dest.QR_ID_Left, dest.QR_Distance_Left, "LEFT");
            }
            if (root.Waypoint_ID_Right.HasValue)
            {
                if (root.Waypoint_ID_Right == dest.WaypointId)
                    return CommandConstructor.Goto(root.WaypointId, dest.QR_ID_Right, dest.QR_Distance_Right, "RIGHT");
            }
            
            return "null";
        }

        // GET: api/manager/robot/5/add/navigationtask/1,4
        /// <summary>
        /// Adds a new navigation task from one waypoint to another.
        /// </summary>
        /// <param name="robotId">RobotID to add the tasks to.</param>
        /// <param name="startWaypointId">Start waypoint ID.</param>
        /// <param name="endWaypointId">End waypoint ID.</param>
        /// <returns>HTTP response action result.</returns>
        [HttpGet("robot/{robotId}/add/navigationtask/{startWaypointId},{endWaypointId}")]
        public async Task<IActionResult> NavigationTasks(int robotId, int startWaypointId, int endWaypointId)
        {
            // TODO (1): Approximate waypoint ID to travel to first (startWaypointId)

            var pathWaypointIds = _navigationApi.GetPathList(startWaypointId, endWaypointId);
            var pathWaypoints = await WaypointsFromListAsync(pathWaypointIds);

            // Initialize counters and objects to add
            var newTasks = new List<TaskModel>();
            var tempTask = new TaskModel();
            int orderNumber = 0;
            
            // Add first waypoint
            tempTask.OrderNumber = orderNumber++;
            // TODO (2): Replace null values, first do TODO (1)
            tempTask.Command = CommandConstructor.Goto(pathWaypointIds[0], null, null, null);
            tempTask.RobotID = robotId;
            newTasks.Add(tempTask);

            for (int i = 1; i<pathWaypoints.Count; i++)
            {
                tempTask = new TaskModel();
                tempTask.OrderNumber = orderNumber++;
                tempTask.Command = TaskCommand(pathWaypoints[i - 1], pathWaypoints[i]);
                tempTask.RobotID = robotId;
                newTasks.Add(tempTask);
            }

            // Add the last Task
            tempTask = new TaskModel();
            tempTask.OrderNumber = orderNumber++;
            tempTask.Command = CommandConstructor.Goto(pathWaypointIds[pathWaypointIds.Count-1], null, null, null);
            tempTask.RobotID = robotId;
            newTasks.Add(tempTask);

            _context.Tasks.AddRange(newTasks);
            await _context.SaveChangesAsync();

            return NoContent();
        }

        // GET: api/manager/robot/5/nexttask
        /// <summary>
        /// Obtains a task for the current robot with order number larger by 1.
        /// Obtains a new task, if there is no next task, returns <see cref="NoContentResult>.
        /// </summary>
        /// <param name="robotId"></param>
        /// <returns>HTTP response action result.</returns>
        [HttpGet("robot/{robotId}/nexttask")]
        public async Task<ActionResult<TaskModel>> NextTask(int robotId)
        {
            int? currentTaskId = _context.CurrentStatuses.SingleOrDefault(s => s.RobotID == robotId).TaskID;

            // Return NotFound() if RobotID cannot be found in Tasks.
            if (!currentTaskId.HasValue)
                return NotFound();

            // Find first (and only) task with current task ID.
            // Throws an exception if not found
            TaskModel currentTask = await _context.Tasks.SingleAsync(t => t.TaskID == currentTaskId && t.RobotID == robotId);

            // Finds a next task, if it exists (next task => current OrderNumber+1.
            // Returns default if not found
            TaskModel? nextTask = await _context.Tasks.SingleOrDefaultAsync(t => t.OrderNumber == currentTask.OrderNumber + 1 && t.RobotID == robotId);

            // Return NoContent() if there is no next task.
            if (nextTask == null)
                NoContent();

            return nextTask;
        }

        // POST: api/manager/robot/5/updatestatus
        /// <summary>
        /// Updates current status of a robot at the specified ID.
        /// Adds a new CurrentState entry if does not exist.
        /// The <paramref name="newStatus"/> should contain ONLY robotId specified in JSON PUT request, 
        /// if the robot awaits (so it has no task).
        /// </summary>
        /// <param name="newStatus"></param>
        /// <returns>HTTP response action result.</returns>
        [HttpPut("robot/{robotId}/updatestatus")]
        public async Task<ActionResult<TaskModel>> UpdateCurrentStatus(int robotId, CurrentStatusModel newStatus)
        {
            // If it is a new record, add. Else, update.
            if (!_context.CurrentStatuses.Any(cs => cs.RobotID == robotId))
                _context.CurrentStatuses.Add(newStatus);
            else
                _context.Entry(newStatus).State = EntityState.Modified;
            
            await _context.SaveChangesAsync();

            // Delete all the old tasks if the current task is null.
            if (newStatus.TaskID == null)
            {
                var oldTasks = _context.Tasks.Where(t => t.RobotID == robotId).ToList();
                _context.Tasks.RemoveRange(oldTasks);
                await _context.SaveChangesAsync();
            }

            return CreatedAtAction("UpdateCurrentStatus", new { id = newStatus.RobotID }, newStatus);
        }

    }
}
