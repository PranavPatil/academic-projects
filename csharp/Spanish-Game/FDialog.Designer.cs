namespace SpanishGame
{
    partial class FDialog
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.inputtext = new System.Windows.Forms.TextBox();
            this.titlelbl = new System.Windows.Forms.Label();
            this.okbtn = new System.Windows.Forms.Button();
            this.cnlbtn = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // inputtext
            // 
            this.inputtext.Location = new System.Drawing.Point(37, 84);
            this.inputtext.Name = "inputtext";
            this.inputtext.Size = new System.Drawing.Size(172, 20);
            this.inputtext.TabIndex = 1;
            // 
            // titlelbl
            // 
            this.titlelbl.Location = new System.Drawing.Point(37, 27);
            this.titlelbl.Name = "titlelbl";
            this.titlelbl.Size = new System.Drawing.Size(172, 42);
            this.titlelbl.TabIndex = 2;
            this.titlelbl.Text = "Listo para iniciar el juego español ! Por favor, introduzca el número de pregunta" +
                "s que usted desea intentar";
            // 
            // okbtn
            // 
            this.okbtn.Location = new System.Drawing.Point(37, 120);
            this.okbtn.Name = "okbtn";
            this.okbtn.Size = new System.Drawing.Size(75, 23);
            this.okbtn.TabIndex = 3;
            this.okbtn.Text = "Ok";
            this.okbtn.UseVisualStyleBackColor = true;
            this.okbtn.Click += new System.EventHandler(this.okbtn_Click);
            // 
            // cnlbtn
            // 
            this.cnlbtn.Location = new System.Drawing.Point(134, 120);
            this.cnlbtn.Name = "cnlbtn";
            this.cnlbtn.Size = new System.Drawing.Size(75, 23);
            this.cnlbtn.TabIndex = 4;
            this.cnlbtn.Text = "Cancel";
            this.cnlbtn.UseVisualStyleBackColor = true;
            this.cnlbtn.Click += new System.EventHandler(this.cnlbtn_Click);
            // 
            // FDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(253, 164);
            this.Controls.Add(this.cnlbtn);
            this.Controls.Add(this.okbtn);
            this.Controls.Add(this.titlelbl);
            this.Controls.Add(this.inputtext);
            this.Name = "FDialog";
            this.Text = "FDialog";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox inputtext;
        private System.Windows.Forms.Label titlelbl;
        private System.Windows.Forms.Button okbtn;
        private System.Windows.Forms.Button cnlbtn;

    }
}