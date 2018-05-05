using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
// Code written by Trevor Walden
namespace HUDEditor
{
    public partial class Form1 : Form // MSDN documentation for windows forms used as reference
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void groupBox1_Enter(object sender, EventArgs e)
        {

        }

        private void CreateElement_Click(object sender, EventArgs e)
        {
            foreach (Control c in Controls)
            {
                if (c.Name == "editor") Controls.Remove(c);
                else if (c.Name == "updateButton") Controls.Remove(c);
            }
            Label newElement = new Label();
            newElement.Name = "SpearCounter";
            newElement.Text = "Spears: 10";
            HUD_View.Controls.Add(newElement);
            newElement.Location = new Point(50, 50);
            newElement.Click += delegate 
            {
                editLabel.Text = "Edit Element ("+newElement.Name+"):";

                TextBox editor = new TextBox();
                editor.Name = "editor";
                Controls.Add(editor);
                editor.Text = newElement.Text;
                editor.Location = new Point(560, 18);

                Button button = new Button();
                Controls.Add(button);
                button.Name = "updateButton";
                button.Text = "Update Element";
                button.Width = 100;
                button.Click += delegate { newElement.Text = editor.Text; };
                button.Location = new Point(670, 16);

            };
        }
    }
}
