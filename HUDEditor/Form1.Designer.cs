namespace HUDEditor
{
    partial class Form1
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
            this.HUD_View = new System.Windows.Forms.GroupBox();
            this.SuspendLayout();
            // 
            // HUD_View
            // 
            this.HUD_View.Location = new System.Drawing.Point(4, 4);
            this.HUD_View.Name = "HUD_View";
            this.HUD_View.Size = new System.Drawing.Size(1280, 720);
            this.HUD_View.TabIndex = 0;
            this.HUD_View.TabStop = false;
            this.HUD_View.Text = "HUD View";
            this.HUD_View.Enter += new System.EventHandler(this.groupBox1_Enter);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1289, 730);
            this.Controls.Add(this.HUD_View);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox HUD_View;
    }
}

