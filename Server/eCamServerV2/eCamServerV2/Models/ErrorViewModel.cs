namespace eCamServerV2.Models
{
    /// <summary>
    /// Model for error view.
    /// </summary>
    public class ErrorViewModel
    {
        public string? RequestId { get; set; }

        public bool ShowRequestId => !string.IsNullOrEmpty(RequestId);
    }
}