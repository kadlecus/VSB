using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Project.Models;
using System.Text.RegularExpressions;
using System.Security.Cryptography.X509Certificates;

namespace Project.Controllers
{
    public class AccountController : Controller
    {
        private readonly Database _database;

        public AccountController(Database database)
        {
            _database = database;
        }

        [HttpGet]
        public IActionResult Register()
        {
            return View();
        }

        [HttpPost]
        public async Task<IActionResult> Register(string Username, string Email, string Password, string PasswordCheck)
        {
            Regex regex = new Regex(@"^[A-Za-z0-9\.]+@{1}[A-Za-z0-9]+\.[A-Za-z0-9]{2,6}$", RegexOptions.IgnoreCase);

            if (!regex.IsMatch(Email))
            {
                TempData["Error"] = "Tento e-mail není platný";
                return View();
            }

            var exist = await _database.GetUserByEmailOrUsername(Email);
            if (exist != null)
            {
                TempData["Error"] = "Tento e-mail je již zaregistrován.";
                return View();
            }

            var usernameTaken = await _database.GetUserByEmailOrUsername(Username);
            if (usernameTaken != null)
            {
                TempData["Error"] = "Toto uživatelské jméno je již obsazené.";
                return View();
            }

            if (Password != PasswordCheck) 
            {
                TempData["Error"] = "Hesla se neshodují.";
                return View();
            }

            string passwordHash = PasswordHasher.HashPassword(Password);

            User user = new User
            {
                Username = Username,
                Email = Email,
                PasswordHash = passwordHash,
                Role = "Registered"
            };

            await _database.CreateUserAsync(user);
            return RedirectToAction("Login");

        }

        [HttpGet]
        public IActionResult Login()
        {
            return View();
        }

        [HttpPost]
        public async Task<IActionResult> Login(string UsernameOrEmail, string Password)
        {
            User? user = await _database.GetUserByEmailOrUsername(UsernameOrEmail);

            if (user == null || !PasswordHasher.VerifyPassword(Password, user.PasswordHash))
            {
                ViewBag.Error = "Neplatné přihlašovací údaje";
                return View();
            }

            HttpContext.Session.SetInt32("UserId", user.Id);
            HttpContext.Session.SetString("Username", user.Username);
            HttpContext.Session.SetString("Role", user.Role);
            HttpContext.Session.SetString("Email", user.Email);

            TempData["Log"] = "Úspěšně jste se přihlásil.";

            return RedirectToAction("Index", "Home");
        }

        public IActionResult Logout()
        {
            
            HttpContext.Session.Clear();
            TempData["Log"] = "Byl jste odhlášen ze svého účtu.";
            return RedirectToAction("Index", "Home");
        
        }

        public async Task<IActionResult> Detail()
        {
            if ((HttpContext.Session.GetString("Role") == "Admin"))
            {
                return RedirectToAction("SearchDetail", "Account");
            }

            if (HttpContext.Session.GetString("Role") != "Registered")
            {
                return RedirectToAction("Login", "Account");
            }

            string? Email = HttpContext.Session.GetString("Email");

            if (string.IsNullOrWhiteSpace(Email))
            {
                return RedirectToAction("Login", "Account");
            }
            await _database.SaveOldReservations();
            ViewBag.UserReservation = await _database.GetParkingReservationAsync(Email);
            ViewBag.UserHistoryReservation = await _database.GetParkingReservationHistoryAsync(Email);

            return View();
        }

        [HttpGet]
       
        public async Task<IActionResult> SearchDetail()
        {
            if (HttpContext.Session.GetString("Role") != "Admin")
            {
                return Unauthorized();
            }
            return View();
        }
        [HttpPost]
       
        public async Task<IActionResult> SearchDetail(string Email)
        {

            if (HttpContext.Session.GetString("Role") != "Admin")
            {
                return Unauthorized();
            }

            if (!string.IsNullOrEmpty(Email))
            {
                ViewBag.AdminReservation = await _database.GetParkingReservationAsync(Email);
                ViewBag.AdminHistoryReservation = await _database.GetParkingReservationHistoryAsync(Email);
            }

            return View();

        }

        [HttpGet]
        public async Task<IActionResult> EditReservation(int id)
        {
            var reservation = await _database.GetReservationByIdAsync(id);

            if (reservation == null)
            {
                TempData["Error"] = "Rezervace nebyla nalezena.";
                if (HttpContext.Session.GetString("Role") == "Admin")
                {
                    return RedirectToAction("SearchDetail", "Account");
                }
                return RedirectToAction("Detail", "Account");
            }

            if (HttpContext.Session.GetString("Role") != "Admin")
            {
                if (reservation.StartTime <= DateTime.Now)
                {
                    TempData["Error"] = "Již započatá rezervace nelze upravovat, kontaktujte prosím admina";
                    return RedirectToAction("Detail", "Account");
                }
            }
          

            if (reservation.EndTime < DateTime.Now)
            {
                TempData["Error"] = "Ukončenou rezervaci nelze upravit.";
                if (HttpContext.Session.GetString("Role") == "Admin")
                {
                    return RedirectToAction("SearchDetail", "Account");
                }
                return RedirectToAction("Detail", "Account");
            }

            return View(reservation);
        }

        [HttpPost]
        public async Task<IActionResult> EditReservation(ParkingReservation updatedReservation)
        {
            var existing = await _database.GetReservationByIdAsync(updatedReservation.Id);

            if (existing == null)
            {
                TempData["Error"] = "Rezervace nebyla nalezena.";
                if (HttpContext.Session.GetString("Role") == "Admin")
                {
                    return RedirectToAction("SearchDetail", "Account");
                }

                return RedirectToAction("Detail", "Account");
            }

            if (existing.StartTime > existing.EndTime)
            {
                TempData["Error"] = "Špatně zadaný čas.";
                if (HttpContext.Session.GetString("Role") == "Admin")
                {
                    return RedirectToAction("SearchDetail", "Account");
                }

                return RedirectToAction("Detail", "Account");
            }

            if (existing.EndTime < DateTime.Now)
            {
                TempData["Error"] = "Ukončenou rezervaci nelze upravit.";
                if (HttpContext.Session.GetString("Role") == "Admin")
                {
                    return RedirectToAction("SearchDetail", "Account");
                }
                return RedirectToAction("Detail", "Account");
            }


            try
            {

                await _database.UpdateReservationAsync(updatedReservation);
                TempData["Success"] = "Rezervace byla úspěšně upravena.";
            }
            catch(InvalidOperationException ex)
            {
                TempData["Error"] = ex.Message;
                if (HttpContext.Session.GetString("Role") == "Admin")
                {
                    return RedirectToAction("SearchDetail", "Account");
                }
                return RedirectToAction("Detail", "Account");
            }


            if (HttpContext.Session.GetString("Role") == "Admin")
            {
                return RedirectToAction("SearchDetail", "Account");
            }
            return RedirectToAction("Detail", "Account");

        }
        public async Task<IActionResult> DeleteReservation(int reservationId)
        {
            var reservation = await _database.GetReservationByIdAsync(reservationId);
            if (reservation == null)
            {
                TempData["Error"] = "Rezervace nebyla nalezena";
                if (HttpContext.Session.GetString("Role") == "Admin")
                {
                    return RedirectToAction("SearchDetail", "Account");
                }
                return RedirectToAction("Detail", "Account");
            }

            var userEmail = HttpContext.Session.GetString("Email");
            if (userEmail != reservation.UserEmail && HttpContext.Session.GetString("Role") != "Admin")
            {
                return Unauthorized();
            }

            await _database.DeleteReservationAsync(reservation.Id);
            TempData["Success"] = "Rezervace byla úspěšně smazána.";
            if (HttpContext.Session.GetString("Role") == "Admin")
            {
                return RedirectToAction("SearchDetail", "Account");
            }
            return RedirectToAction("Detail", "Account");

        }
    }
}
