using eCamServerV2.Models;
using Microsoft.AspNetCore.Mvc;
using Newtonsoft.Json;
using System.Diagnostics;

namespace eCamServerV2.Controllers
{
    public class HomeController : Controller
    {
        // GET: Home
        public IActionResult Index()
        {
            List<WaypointModel> waypoints = GetAllWaypoints();
            return View(waypoints);
        }


        private static List<WaypointModel> GetAllWaypoints()
        {
            List<WaypointModel> waypoints = new List<WaypointModel>();
            string apiUrl = "http://localhost:5226/api/Waypoints";

            HttpClient client = new HttpClient();
            HttpResponseMessage response = client.GetAsync(apiUrl).Result;
            if (response.IsSuccessStatusCode)
            {
                waypoints = JsonConvert.DeserializeObject<List<WaypointModel>>(response.Content.ReadAsStringAsync().Result);
            }

            return waypoints;
        }
    }
}