using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using eCamServerV2.Data;
using eCamServerV2.Models;
using System.ComponentModel.DataAnnotations.Schema;

namespace eCamServerV2.Controllers
{
    /// <summary>
    /// API controller for the <see cref="DbContextECam.Waypoints"/> table.
    /// </summary>
    [Route("api/Waypoints")]
    [ApiController]
    public class WaypointsAPIController : ControllerBase
    {
        private readonly DbContextECam _context;

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="context">Current application database context class, that inherits from <see cref="DbContext"/>.</param>
        public WaypointsAPIController(DbContextECam context)
        {
            _context = context;
        }

        // GET: api/Waypoints/getall
        /// <summary>
        /// Gets all waypoints in the table.
        /// </summary>
        /// <returns>All the waypoints in a JSON format.</returns>
        [HttpGet("getall")]
        public async Task<ActionResult<IEnumerable<WaypointModel>>> GetWaypoints()
        {
            return await _context.Waypoints.ToListAsync();
        }

        // GET: api/Waypoints/get/5
        /// <summary>
        /// Gets a waypoint at a specified <paramref name="id"/>.
        /// </summary>
        /// <param name="id">ID of a waypoint to get.</param>
        /// <returns>Waypoint model with a specified <paramref name="id"/> in a JSON format.</returns>
        [HttpGet("get/{id}")]
        public async Task<ActionResult<WaypointModel>> GetWaypoint(int id)
        {
            var waypoint = await _context.Waypoints.FindAsync(id);

            if (waypoint == null)
            {
                return NotFound();
            }

            return waypoint;
        }

        // PUT: api/Waypoints/edit/5
        /// <summary>
        /// Edits existing waypoint. 
        /// </summary>
        /// <param name="id">ID of a waypoint to edit.</param>
        /// <param name="waypoint">Must be in application/json format specifying wanted members of Waypoint model class. Must include ID.</param>
        /// <returns>
        /// <see cref="NotFoundResult"/> if not found.
        /// <see cref="BadRequestResult"/> if id != <paramref name="waypoint"/>.WaypointId.
        /// <see cref="NoContentResult"/> if successful.
        /// </returns>
        [HttpPut("edit/{id}")]
        public async Task<IActionResult> PutWaypoint(int id, WaypointModel waypoint)
        {
            if (id != waypoint.WaypointId)
            {
                return BadRequest();
            }

            _context.Entry(waypoint).State = EntityState.Modified;

            try
            {
                await _context.SaveChangesAsync();
            }
            catch (DbUpdateConcurrencyException)
            {
                if (!WaypointExists(id))
                {
                    return NotFound();
                }
                else
                {
                    throw;
                }
            }

            return NoContent();
        }

        //POST: api/Waypoints/add
        /// <summary>
        /// Adds a new Waypoint to the Waypoints table.
        /// </summary>
        /// <param name="waypointData">Must be in application/json format specifying wanted members of Waypoint model class.</param>
        /// <returns><see cref="CreatedAtActionResult"/> if successful.</returns>
        [HttpPost("add")]
        public async Task<ActionResult<WaypointModel>> PostWaypoint(WaypointModel waypoint)
        {
            _context.Waypoints.Add(waypoint);
            await _context.SaveChangesAsync();

            return CreatedAtAction("GetWaypoint", new { id = waypoint.WaypointId }, waypoint);
        }

        // DELETE: api/Waypoints/delete/5
        /// <summary>
        /// Deletes a waypoint at specified <paramref name="id"/>.
        /// </summary>
        /// <param name="id">ID of a waypoint to delete.</param>
        /// <returns>
        /// <see cref="NotFoundResult"/> if not found.
        /// <see cref="NoContentResult"/> if successful.
        /// </returns>
        [HttpDelete("delete/{id}")]
        public async Task<IActionResult> DeleteWaypoint(int id)
        {
            var waypoint = await _context.Waypoints.FindAsync(id);
            if (waypoint == null)
            {
                return NotFound();
            }

            _context.Waypoints.Remove(waypoint);
            await _context.SaveChangesAsync();

            return NoContent();
        }

        /// <summary>
        /// Evaluates if a waypoint at the specified <paramref name="id"/> exists.
        /// </summary>
        /// <param name="id">ID of a waypoint to check.</param>
        /// <returns>Boolean if a waypoint exists.</returns>
        private bool WaypointExists(int id)
        {
            return _context.Waypoints.Any(e => e.WaypointId == id);
        }
    }
}
