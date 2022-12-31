using Microsoft.AspNetCore.Mvc;
using eCamServerV2.Utilities;
using eCamServerV2.Models;
using Newtonsoft.Json;
using eCamServerV2.Data;

namespace eCamServerV2.Controllers
{
    /// <summary>
    /// API controller for the navigation.
    /// </summary>
    [Route("api/navigation")]
    [ApiController]
    public class NavigationAPIController : ControllerBase
    {
        private Pathfinder _pathfinder;
        private readonly DbContextECam _context;

        /// <summary>
        /// Constructor.
        /// </summary>
        public NavigationAPIController(DbContextECam context)
        {
            _context = context;
            // Initialize the pathfinder to the current WaypointMap.
            _pathfinder = new Pathfinder(_context.Waypoints.ToList());
        }

        // GET api/navigation/getpath/1,5
        /// <summary>
        /// Gets the path using <see cref="Pathfinder.GetPath(int, int)"/>.
        /// </summary>
        /// <param name="idStart">ID of a waypoint to start at.</param>
        /// <param name="idEnd">ID of a waypoint to finish at.</param>
        /// <returns>String representation of the path.</returns>
        [HttpGet("getpath/{idStart},{idEnd}")]
        public string GetPathString(int idStart, int idEnd)
        {
            List<int> path = _pathfinder.GetPath(idStart, idEnd);

            // Convert the list of ints to a string
            string stringPath = "";
            foreach(var id in path)
            {
                stringPath += id + ",";
            }
            return stringPath;
        }

        public List<int> GetPathList(int idStart, int idEnd)
        {
            return _pathfinder.GetPath(idStart, idEnd);
        }

        public List<int> GetPathList()
        {
            throw new NotImplementedException();
        }
    }
}
