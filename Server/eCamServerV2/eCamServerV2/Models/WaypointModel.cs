using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace eCamServerV2.Models
{
    /// <summary>
    /// Model for waypoint row
    /// </summary>
    [Table("waypoints")]
    public class WaypointModel
    {
        [Key]
        public int WaypointId { get; set; }

        public int? QR_ID_Left { get; set; }
        public int? QR_ID_Right { get; set; }
        public int? QR_ID_Top { get; set; }
        public int? QR_ID_Bottom { get; set; }
        public string? QR_Distance_Left { get; set; }
        public string? QR_Distance_Right { get; set; }
        public string? QR_Distance_Top { get; set; }
        public string? QR_Distance_Bottom { get; set; }
        public int? Waypoint_ID_Left { get; set; }
        public int? Waypoint_ID_Right { get; set; }
        public int? Waypoint_ID_Top { get; set; }
        public int? Waypoint_ID_Bottom { get; set; }
        public string? Waypoint_Distance_Left { get; set; }
        public string? Waypoint_Distance_Right { get; set; }
        public string? Waypoint_Distance_Top { get; set; }
        public string? Waypoint_Distance_Bottom { get; set; }
    }
}
