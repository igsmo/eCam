using eCamServerV2.Models;
using Microsoft.EntityFrameworkCore;
using System.ComponentModel.DataAnnotations.Schema;

namespace eCamServerV2.Data
{
    /// <summary>
    /// Current application database context.
    /// </summary>
    public class DbContextECam : DbContext
    {
        public DbContextECam(DbContextOptions<DbContextECam> options) : base(options)
        {

        }

        /// <summary>
        /// Table containing <see cref="WaypointModel"/>s.
        /// </summary>
        public DbSet<WaypointModel> Waypoints { get; set; }

        /// <summary>
        /// Table containing <see cref="CurrentStatusModel"/>s.
        /// </summary>
        public DbSet<CurrentStatusModel> CurrentStatuses { get; set; }

        /// <summary>
        /// Table containing <see cref="TaskModel"/>s.
        /// </summary>
        public DbSet<TaskModel> Tasks { get; set; }
    }
}
