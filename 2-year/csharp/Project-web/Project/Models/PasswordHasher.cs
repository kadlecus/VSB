using System.Security.Cryptography;
using System.Text;

namespace Project.Models
{
    public static class PasswordHasher
    {
        public static string HashPassword(string password)
        {
            using (SHA256 sha256 = SHA256.Create())
            {
                byte[] input = Encoding.UTF8.GetBytes(password);
                byte[] output = sha256.ComputeHash(input);

                StringBuilder stringBuilder = new StringBuilder();

                foreach (byte b in output)
                {
                    stringBuilder.Append(b.ToString("x2"));
                }

                return stringBuilder.ToString();
            }
        }

        public static bool VerifyPassword(string inputPassWord, string storedHash)
        {
            using (SHA256 sha256 = SHA256.Create())
            {

                byte[] hash = sha256.ComputeHash(Encoding.UTF8.GetBytes(inputPassWord));
                StringBuilder stringBuilder = new StringBuilder();
                foreach (byte b in hash)
                {
                    stringBuilder.Append(b.ToString("x2"));
                }
                return stringBuilder.ToString() == storedHash;
            }

        }
    }
}
