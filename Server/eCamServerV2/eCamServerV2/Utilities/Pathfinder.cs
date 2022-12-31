using eCamServerV2.Models;
using WaypointPathfinding;

namespace eCamServerV2.Utilities
{
    /// <summary>
    /// Class implementing and customizing <see cref="WaypointPathfinding"/> pathfinder and data structures.
    /// </summary>
    public class Pathfinder
    {
        /// <summary>
        /// A list containing all the <see cref="WaypointModel"/>s int the <see cref="_waypointMap"/>.
        /// </summary>
        private List<WaypointModel> _waypoints;
        /// <summary>
        /// <see cref="WaypointMap"/> containing current map layout.
        /// </summary>
        private WaypointMap _waypointMap;
        /// <summary>
        /// Private variable for the pathfinder algorithm in use.
        /// </summary>
        private DijkstraPathfinder _pathfinder;


        /// <summary>
        /// Getter for <see cref="_waypoints"/>.
        /// </summary>
        public List<WaypointModel> Waypoints { get { return _waypoints; } }

        public Pathfinder(List<WaypointModel> waypoints)
        {
            UpdateWaypoints(waypoints);
        }

        /// <summary>
        /// Converts list of <see cref="WaypointModel"/>s to the list of strings accepted by <see cref="WaypointMapParser"/> to convert to <see cref="WaypointMap"/>.
        /// </summary>
        /// <param name="waypoints">List of <see cref="WaypointModel"/>s to convert from.</param>
        /// <returns>List of strings accepted by <see cref="WaypointMapParser"/>.</returns>
        /// <exception cref="NullReferenceException">Throws when there exists a connection to a waypointID, but the distance is not specified.</exception>
        private static List<string> WaypointsListToStrings(List<WaypointModel> waypoints)
        {
            // Initialize result
            List<string> waypointStrings = new List<string>();
            foreach (var waypoint in waypoints)
            {
                // Initialize string-representations of arrays
                string connectionIds = "[";
                string connectionDistances = "[";
                string tempString = waypoint.WaypointId.ToString() + ",";
                // Add all the waypoint connections if a connection exists.
                // Checks if the distance info exists.
                if (waypoint.Waypoint_ID_Left != null)
                {
                    connectionIds += waypoint.Waypoint_ID_Left.ToString() + ",";
                    if (waypoint.Waypoint_Distance_Left == null)
                        throw new NullReferenceException();
                    connectionDistances += waypoint.Waypoint_Distance_Left.ToString() + ",";
                }
                if (waypoint.Waypoint_ID_Right != null)
                {
                    connectionIds += waypoint.Waypoint_ID_Right.ToString() + ",";
                    if (waypoint.Waypoint_Distance_Right== null)
                        throw new NullReferenceException();
                    connectionDistances += waypoint.Waypoint_Distance_Right.ToString() + ",";
                }
                if (waypoint.Waypoint_ID_Top != null)
                {
                    connectionIds += waypoint.Waypoint_ID_Top.ToString() + ",";
                    if (waypoint.Waypoint_Distance_Top == null)
                        throw new NullReferenceException();
                    connectionDistances += waypoint.Waypoint_Distance_Top.ToString() + ",";
                }
                if (waypoint.Waypoint_ID_Bottom != null)
                {
                    connectionIds += waypoint.Waypoint_ID_Bottom.ToString() + ",";
                    if (waypoint.Waypoint_Distance_Bottom == null)
                        throw new NullReferenceException();
                    connectionDistances += waypoint.Waypoint_Distance_Bottom.ToString() + ",";
                }
                // Removes last ',' and closes string-representation of the arrays
                connectionIds = connectionIds.Substring(0, connectionIds.Length-1) + "]";
                connectionDistances = connectionDistances.Substring(0, connectionDistances.Length - 1) + "]";

                // Adds current string to the final result
                waypointStrings.Add(waypoint.WaypointId.ToString() + ";" + connectionIds + ";" + connectionDistances);
            }

            return waypointStrings;
        }

        /// <summary>
        /// Updates current <see cref="Waypoints"/> and pathfinder information.
        /// </summary>
        /// <param name="waypoints">New Waypoints list.</param>
        public void UpdateWaypoints(List<WaypointModel> waypoints)
        {
            _waypoints = waypoints;
            _waypointMap = WaypointMapParser.ParseArrayOfStrings(WaypointsListToStrings(_waypoints));
            _pathfinder = new DijkstraPathfinder(_waypointMap);
        }

        /// <summary>
        /// Gets the path using <see cref="DijkstraPathfinder.GetPath(WaypointNode, WaypointNode)"/>.
        /// </summary>
        /// <param name="idStart">ID of a waypoint to start at.</param>
        /// <param name="idEnd">ID of a waypoint to finish at.</param>
        /// <returns>List-of-WaypointIDs representation of the path.</returns>
        /// <exception cref="NullReferenceException">Throws when there does not exist either waypoint at <paramref name="idStart"/> or <paramref name="idEnd"/>.</exception>
        public List<int> GetPath(int idStart, int idEnd)
        {
            var pathIds = new List<int>();

            // Get Waypoints where ID == idStart and where ID == idEnd
            var startWaypoint = _waypointMap.Waypoints.FirstOrDefault(x => x.Id == idStart, null);
            var endWaypoint = _waypointMap.Waypoints.FirstOrDefault(x => x.Id == idEnd, null);
            
            // Throw when either is not found
            if (startWaypoint == null || endWaypoint == null)
                throw new NullReferenceException();

            var path = _pathfinder.GetPath(startWaypoint, endWaypoint);

            // Add nodes to the final result
            foreach (var node in path)
            {
                pathIds.Add(node.Id);
            }

            return pathIds;
        }
    }
}
