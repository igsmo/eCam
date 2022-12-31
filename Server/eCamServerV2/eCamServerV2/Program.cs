using eCamServerV2.Controllers;
using eCamServerV2.Data;
using Microsoft.EntityFrameworkCore;

var builder = WebApplication.CreateBuilder(args);

// Add services to the container.
builder.Services.AddControllersWithViews();

var connectionString = builder.Configuration.GetConnectionString("eCamDb");
builder.Services.AddDbContext<DbContextECam>(options => options.UseMySql(
    connectionString, ServerVersion.AutoDetect(connectionString)
    ));

builder.Services.AddHttpClient();

// Add NavigationAPI reference
builder.Services.AddScoped<NavigationAPIController, NavigationAPIController>();

var app = builder.Build();

// Configure the HTTP request pipeline.
if (!app.Environment.IsDevelopment())
{
    app.UseExceptionHandler("/Home/Error");
    // The default HSTS value is 30 days. You may want to change this for production scenarios, see https://aka.ms/aspnetcore-hsts.
    app.UseHsts();
}

app.UseStaticFiles();

app.UseRouting();

app.UseAuthorization();

app.MapControllerRoute(
    name: "default",
    pattern: "{controller=Home}/{action=Index}/{id?}");

app.Run();
