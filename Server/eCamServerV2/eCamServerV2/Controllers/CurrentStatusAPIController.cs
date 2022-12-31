using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using eCamServerV2.Data;
using eCamServerV2.Models;
using System.ComponentModel.DataAnnotations.Schema;

namespace eCamServerV2.Controllers
{    /// <summary>
     /// API controller for the <see cref="DbContextECam.CurrentStatuses"/> table.
     /// </summary>
    [Route("api/CurrentStatus")]
    [ApiController]
    public class CurrentStatusAPIController : ControllerBase
    {
        private readonly DbContextECam _context;

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="context">Current application database context class, that inherits from <see cref="DbContext"/>.</param>
        public CurrentStatusAPIController(DbContextECam context)
        {
            _context = context;
        }

        // GET: api/CurrentStatus/getall
        /// <summary>
        /// Gets all current status in the table.
        /// </summary>
        /// <returns>All the current status in a JSON format.</returns>
        [HttpGet("getall")]
        public async Task<ActionResult<IEnumerable<CurrentStatusModel>>> GetCurrentStatus()
        {
            return await _context.CurrentStatuses.ToListAsync();
        }

        // GET: api/CurrentStatus/get/5
        /// <summary>
        /// Gets a current status at a specified <paramref name="id"/>.
        /// </summary>
        /// <param name="id">ID of a current task to get.</param>
        /// <returns>CurrentStatus model with a specified <paramref name="id"/> in a JSON format.</returns>
        [HttpGet("get/{id}")]
        public async Task<ActionResult<CurrentStatusModel>> GetCurrentStatus(int id)
        {
            var currentStatus = await _context.CurrentStatuses.FindAsync(id);

            if (currentStatus == null)
            {
                return NotFound();
            }

            return currentStatus;
        }

        // PUT: api/CurrentStatus/edit/5
        /// <summary>
        /// Edits existing current status. 
        /// </summary>
        /// <param name="id">ID of a current status to edit.</param>
        /// <param name="currentStatus">Must be in application/json format specifying wanted members of Waypoint model class. Must include ID.</param>
        /// <returns>
        /// <see cref="NotFoundResult"/> if not found.
        /// <see cref="BadRequestResult"/> if id != <paramref name="waypoint"/>.WaypointId.
        /// <see cref="NoContentResult"/> if successful.
        /// </returns>
        [HttpPut("edit/{id}")]
        public async Task<IActionResult> PutCurrentStatus(int id, CurrentStatusModel currentStatus)
        {
            if (id != currentStatus.RobotID)
            {
                return BadRequest();
            }

            _context.Entry(currentStatus).State = EntityState.Modified;

            try
            {
                await _context.SaveChangesAsync();
            }
            catch (DbUpdateConcurrencyException)
            {
                if (!CurrentStatusExists(id))
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

        // POST: api/CurrentStatus/add
        /// <summary>
        /// Adds a new current status to the CurrentStatus table.
        /// </summary>
        /// <param name="waypointData">Must be in application/json format specifying wanted members of Waypoint model class.</param>
        /// <returns><see cref="CreatedAtActionResult"/> if successful.</returns>
        [HttpPost("add")]
        public async Task<ActionResult<CurrentStatusModel>> PostCurrentStatus(CurrentStatusModel currentStatus)
        {
            _context.CurrentStatuses.Add(currentStatus);
            await _context.SaveChangesAsync();

            return CreatedAtAction("GetCurrentStatus", new { id = currentStatus.RobotID }, currentStatus);
        }

        // DELETE: api/CurrentStatus/delete/5
        /// <summary>
        /// Deletes a current status at specified <paramref name="id"/>.
        /// </summary>
        /// <param name="id">ID of a current status to delete.</param>
        /// <returns>
        /// <see cref="NotFoundResult"/> if not found.
        /// <see cref="NoContentResult"/> if successful.
        /// </returns>
        [HttpDelete("delete/{id}")]
        public async Task<IActionResult> DeleteCurrentStatus(int id)
        {
            var currentStatus = await _context.CurrentStatuses.FindAsync(id);
            if (currentStatus == null)
            {
                return NotFound();
            }

            _context.CurrentStatuses.Remove(currentStatus);
            await _context.SaveChangesAsync();

            return NoContent();
        }

        /// <summary>
        /// Evaluates if a current status at the specified <paramref name="id"/> exists.
        /// </summary>
        /// <param name="id">ID of a current status to check.</param>
        /// <returns>Boolean if a current status exists.</returns>
        private bool CurrentStatusExists(int id)
        {
            return _context.CurrentStatuses.Any(e => e.RobotID == id);
        }
    }
}
