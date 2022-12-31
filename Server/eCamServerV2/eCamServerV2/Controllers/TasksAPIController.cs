using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using eCamServerV2.Data;
using eCamServerV2.Models;

namespace eCamServerV2.Controllers
{
    /// <summary>
    /// API controller for the <see cref="DbContextECam.Tasks"/> table.
    /// </summary>
    [Route("api/Tasks")]
    [ApiController]
    public class TasksAPIController : ControllerBase
    {
        private readonly DbContextECam _context;

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="context">Current application database context class, that inherits from <see cref="DbContext"/>.</param>
        public TasksAPIController(DbContextECam context)
        {
            _context = context;
        }

        // GET: api/Tasks/getall
        /// <summary>
        /// Gets all tasks in the table.
        /// </summary>
        /// <returns>All the tasks in a JSON format.</returns>
        [HttpGet("getall")]
        public async Task<ActionResult<IEnumerable<TaskModel>>> GetTask()
        {
            return await _context.Tasks.ToListAsync();
        }
        
        // GET: api/Tasks/get/5
        /// <summary>
        /// Gets a task at a specified <paramref name="id"/>.
        /// </summary>
        /// <param name="id">ID of a task to get.</param>
        /// <returns>Task model with a specified <paramref name="id"/> in a JSON format.</returns>
        [HttpGet("get/{id}")]
        public async Task<ActionResult<TaskModel>> GetTask(int id)
        {
            var task = await _context.Tasks.FindAsync(id);

            if (task == null)
            {
                return NotFound();
            }

            return task;
        }

        // PUT: api/Tasks/edit/5
        /// <summary>
        /// Edits existing task. 
        /// </summary>
        /// <param name="id">ID of a task to edit.</param>
        /// <param name="task">Must be in application/json format specifying wanted members of Task model class. Must include ID.</param>
        /// <returns>
        /// <see cref="NotFoundResult"/> if not found.
        /// <see cref="BadRequestResult"/> if id != <paramref name="waypoint"/>.WaypointId.
        /// <see cref="NoContentResult"/> if successful.
        /// </returns>
        [HttpPut("edit/{id}")]
        public async Task<IActionResult> PutTask(int id, TaskModel task)
        {
            if (id != task.TaskID)
            {
                return BadRequest();
            }

            _context.Entry(task).State = EntityState.Modified;

            try
            {
                await _context.SaveChangesAsync();
            }
            catch (DbUpdateConcurrencyException)
            {
                if (!TaskExists(id))
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

        // POST: api/Tasks/add
        /// <summary>
        /// Adds a new Task to the Tasks table.
        /// </summary>
        /// <param name="task">Must be in application/json format specifying wanted members of TaskModel model class.</param>
        /// <returns><see cref="CreatedAtActionResult"/> if successful.</returns>
        [HttpPost("add")]
        public async Task<ActionResult<TaskModel>> PostTask(TaskModel task)
        {
            _context.Tasks.Add(task);
            await _context.SaveChangesAsync();

            return CreatedAtAction("GetTask", new { id = task.TaskID }, task);
        }

        // DELETE: api/Tasks/delete/5
        /// <summary>
        /// Deletes a task at specified <paramref name="id"/>.
        /// </summary>
        /// <param name="id">ID of a task to delete.</param>
        /// <returns>
        /// <see cref="NotFoundResult"/> if not found.
        /// <see cref="NoContentResult"/> if successful.
        /// </returns>
        [HttpDelete("delete/{id}")]
        public async Task<IActionResult> DeleteTask(int id)
        {
            var task = await _context.Tasks.FindAsync(id);
            if (task == null)
            {
                return NotFound();
            }

            _context.Tasks.Remove(task);
            await _context.SaveChangesAsync();

            return NoContent();
        }
        /// <summary>
        /// Evaluates if a task at the specified <paramref name="id"/> exists.
        /// </summary>
        /// <param name="id">ID of a task to check.</param>
        /// <returns>Boolean if a task exists.</returns>
        private bool TaskExists(int id)
        {
            return _context.Tasks.Any(e => e.TaskID == id);
        }
    }
}
