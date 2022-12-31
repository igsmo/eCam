namespace eCamServerV2.Utilities
{


    public static class CommandConstructor
    {
        private static class Command
        {
            public const string GOTO = "goto [{waypointId}, {QR_ID_InFront}, {QR_Distance_InFront}, {QR_Orient_InFront}]";
        }

        public static string Goto(int waypointId, int? qrId, string? qrDistance, string? qrOrient)
        {
            var command = Command.GOTO;
            command = command.Replace("{waypointId}", waypointId.ToString());

            if (qrId.HasValue)
                command = command.Replace("{QR_ID_InFront}", qrId.ToString());
            else
                command = command.Replace("{QR_ID_InFront}", "null");

            if (qrDistance != null)
                command = command.Replace("{QR_Distance_InFront}", qrDistance.ToString());
            else
                command = command.Replace("{QR_Distance_InFront}", "null");

            if (qrOrient != null)
                command = command.Replace("{QR_Orient_InFront}", qrOrient.ToString());
            else
                command = command.Replace("{QR_Orient_InFront}", "null");

            return command;
        }
    }

}

