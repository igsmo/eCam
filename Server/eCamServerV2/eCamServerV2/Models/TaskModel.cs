using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Diagnostics.CodeAnalysis;

namespace eCamServerV2.Models
{
    /// <summary>
    /// Model for task row.
    /// </summary>
    [Table("tasks")]
    public class TaskModel
    {
        [Key]
        public int TaskID { get; set; }

        [Required, NotNull]
        public int RobotID { get; set; }

        public int? OrderNumber { get; set; }

        public string? Command { get; set; }
    }
}
