using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Diagnostics.CodeAnalysis;

namespace eCamServerV2.Models
{
    /// <summary>
    /// Model for CurrentStatuses row
    /// </summary>
    [Table("current_status")]
    public class CurrentStatusModel
    {
        [Key]
        public int RobotID { get; set; }

        public int? TaskID { get; set; }

        public int? QR_ID_InFront { get; set; }
        public string? QR_Distance_InFront { get; set; }
    }
}
