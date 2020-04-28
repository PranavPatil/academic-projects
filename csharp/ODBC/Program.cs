using System;
using System.Data.Odbc;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;

namespace ConsoleApplication4
{
    class Program
    {
        private System.Data.Odbc.OdbcConnection OdbcCon;
        private System.Data.Odbc.OdbcCommand OdbcCom;
        private System.Data.Odbc.OdbcDataReader OdbcDR;
        private string ConStr;

        static void Main(string[] args)
        {
            string Database = "mydb";
            Program prog = new Program();
            prog.Connect(Database);
            prog.ListTables(Database);
            prog.Disconnect();
        }

        private void Connect(string Database)
        {
            string Server = "MyPC\\SQLEXPRESS";
            string Username = "sa";
            string Password = "";

            ConStr = "Driver={SQL Server};";
            ConStr += "Server=" + Server + ";";
            ConStr += "User ID=" + Username + ";";
            ConStr += "Password=" + Password + ";";
            ConStr += "Initial Catalog=" + Database + ";";
 
			OdbcCon = new System.Data.Odbc.OdbcConnection(ConStr);

			try
			{
				Console.WriteLine("Openning connection...\r\n");
				if (OdbcCon.State == ConnectionState.Closed)
				{
					OdbcCon.Open();
				}
				Console.WriteLine("ODBC Connection State = " + OdbcCon.State + "\r\n");
			}
			catch (System.Data.Odbc.OdbcException Ex)
			{
				Console.WriteLine(Ex.Message + "\r\n");
				Console.WriteLine("Could not access the database.\r\nPlease make sure you completed the fields with the correct information and try again.\r\n\r\nMore details:\r\n" + Ex.Message);
			}
        }


        private void ListTables(string Database)
		{
			if (OdbcCon.State == ConnectionState.Open)
			{
                OdbcCom = new System.Data.Odbc.OdbcCommand("SELECT * FROM [mydb].[dbo].[user]", OdbcCon);
				OdbcDR = OdbcCom.ExecuteReader();
                Console.WriteLine("Content in Table [" + Database + "].[dbo].[user] :\r\n");
				while (OdbcDR.Read())
				{
                    Console.WriteLine("Values >> " + OdbcDR[0] + " , " + OdbcDR[1] + " , " + OdbcDR[2] + " , " + OdbcDR[3] + " , " + OdbcDR[4] + "\r\n");
				}
			}
		}

		private void Disconnect()
		{
			if (OdbcCon.State == ConnectionState.Open)
			{
				OdbcCon.Close();
			}
		}
    }
}
