using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
// Code written by Trevor Walden with reference from MSDN documentation
namespace HUDEditor
{
    public partial class Form1 : Form // MSDN documentation for windows forms used as reference
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

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
            newElement.AutoSize = true;
            newElement.Name = "SpearCounter";
            newElement.Text = "Spears: 10";
            HUD_View.Controls.Add(newElement);
            newElement.Cursor = Cursors.SizeAll;
            newElement.Location = new Point(50, 50);
            newElement.Enabled = true;
            newElement.AllowDrop = false; // Use this to do drag dropping

            // Allow editing on click
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

            newElement.MouseDown += delegate
            {
                //newElement.DoDragDrop(newElement, DragDropEffects.Move);
                //if ()
                newElement.AllowDrop = true;
            };

            newElement.MouseMove += delegate
            {
                if (newElement.AllowDrop)
                {
                    Point p = PointToClient(new Point(Cursor.Position.X - 5, Cursor.Position.Y - 48));
                    int l = 0;
                    int r = HUD_View.Size.Width;
                    int t = 0;
                    int b = HUD_View.Size.Height;
                    if (p.X+newElement.Width > r)
                    {
                        p.X = r - newElement.Width;
                    }
                    if (p.X < l)
                    {
                        p.X = l;
                    }
                    if (p.Y+newElement.Height > b)
                    {
                        p.Y = b-newElement.Height;
                    }
                    if (p.Y < t)
                    {
                        p.Y = t;
                    }
                    newElement.Location = p;

                }
            };

            newElement.MouseUp += delegate
            {
                //newElement.DoDragDrop(newElement, DragDropEffects.Move);
                newElement.AllowDrop = false;
            };
        }

        private void exitButton_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void exportButton_Click(object sender, EventArgs e)
        {
            FileStream fstream;
            try
            {
                fstream = new FileStream("..\\..\\..\\Assets\\GameUI.json", FileMode.CreateNew, FileAccess.ReadWrite);
            }
            catch (IOException ex)
            {
                DialogResult dialog = MessageBox.Show("A UI file already exists, are you sure you want to overwrite it?", "Overwrite File", MessageBoxButtons.OKCancel);
                if (dialog == DialogResult.OK)
                {
                    fstream = new FileStream("..\\..\\..\\Assets\\GameUI.json", FileMode.Create, FileAccess.ReadWrite);
                }
                else return;
            }

            // Save the json data

            fstream.Close();
            MessageBox.Show("UI File successfully saved.", "Save File", MessageBoxButtons.OK);
        }
    }
}
