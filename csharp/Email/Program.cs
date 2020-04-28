using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Mail;

namespace email
{
    class Program
    {
        static void Main(string[] args)
        {
            MailMessage mail = new MailMessage();
            SmtpClient SmtpServer = new SmtpClient("smtp.gmail.com");

            mail.From = new MailAddress("web.duke@gmail.com");
            mail.To.Add("pranav@gmail.com");
            mail.Subject = "Test Mail";
            mail.Body = "This is for testing SMTP mail from GMAIL";

            SmtpServer.Port = 587;
            SmtpServer.Credentials = new System.Net.NetworkCredential("duke.edu", "yourpassword");
            SmtpServer.EnableSsl = true;

            SmtpServer.Send(mail);

        }
    }
}
