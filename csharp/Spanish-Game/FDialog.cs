using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SpanishGame
{
    public partial class FDialog : Form
    {
        string iptext = null;

        public FDialog()
        {
            InitializeComponent();
            inputtext.Text = "";
        }

        private void okbtn_Click(object sender, EventArgs e)
        {
            iptext = inputtext.Text;
            this.Hide();
        }

        public string getInputText() 
        {
            return iptext;
        }

        public void resetInputText()
        {
            inputtext.Text = "";
        }

        public void setIpTextFocus()
        {
            inputtext.Focus();
        }

        private void cnlbtn_Click(object sender, EventArgs e)
        {
            iptext = null;
            this.Hide(); 
        }
    }
}
