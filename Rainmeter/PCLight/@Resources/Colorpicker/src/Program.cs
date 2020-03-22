using System;
using System.Drawing;
using System.Windows.Forms;

namespace cpicker
{
    class Program
    {
        static void Main(string[] args)
        {
            var colorDialog = new System.Windows.Forms.ColorDialog();
            colorDialog.FullOpen = true;
            colorDialog.Color = Color.Red;

            if (colorDialog.ShowDialog() == DialogResult.OK)
            {
                var val = Math.Round(colorDialog.Color.GetHue() / 360 * 255);
                Console.WriteLine(val);
            }
        }
    }
}
