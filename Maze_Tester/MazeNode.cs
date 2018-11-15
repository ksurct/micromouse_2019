using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Drawing;

namespace howto_maze
{
    class MazeNode
    {
        public const int North = 0;
        public const int South = North + 1;
        public const int East = South + 1;
        public const int West = East + 1;

        public int[] specialEdges = { -1, -1, -1, -1 };
        // The node's neighbors in order North, South, East, West.
        public MazeNode[] Neighbors = new MazeNode[4];

        public bool[] dontDraw = new bool[4];

        // The predecessor in the spanning tree.
        public MazeNode Predecessor = null;

        // The node's bounds.
        public Rectangle Bounds;

        // Return this node's center.
        public Point Center
        {
            get
            {
                int x = Bounds.Left + Bounds.Width / 2;
                int y = Bounds.Top + Bounds.Height / 2;
                return new Point(x, y);
            }
        }

        // Constructor.
        public MazeNode(int x, int y, int wid, int hgt)
        {
            Bounds = new Rectangle(x, y, wid, hgt);
        }

        // Draw the node's bounding box.
        public void DrawBoundingBox(Graphics gr, Pen pen)
        {
            gr.DrawRectangle(pen,
                Bounds.Left + 1, Bounds.Y + 1,
                Bounds.Width - 2, Bounds.Height - 2);
        }

        // Draw a circle at the node's center.
        public void DrawCenter(Graphics gr, Brush brush)
        {
            int cx = Bounds.Left + Bounds.Width / 2;
            int cy = Bounds.Top + Bounds.Height / 2;
            gr.FillEllipse(brush, cx - 2, cy - 2, 4, 4);
        }

        // Draw a link to the node's predecessor.
        public void DrawPredecessorLink(Graphics gr, Pen pen)
        {
            if ((Predecessor != null) && (Predecessor != this))
                gr.DrawLine(pen, Center, Predecessor.Center);
        }

        // Draw links to the node's neighbors.
        public void DrawNeighborLinks(Graphics gr, Pen pen)
        {
            foreach (MazeNode neighbor in Neighbors)
            {
                if (neighbor != null)
                {
                    int dx = (int)(0.4 * (neighbor.Center.X - Center.X));
                    int dy = (int)(0.4 * (neighbor.Center.Y - Center.Y));
                    Point pt = new Point(Center.X + dx, Center.Y + dy);
                    gr.DrawLine(pen, Center, pt);
                }
            }
        }

        public bool AreAdjacent(MazeNode first, MazeNode second, int direction)
        {
            if (specialEdges[direction] >= 0)
            {
                return specialEdges[direction] == 1;
            }
            return ((second == null) || ((second.Predecessor != first) && (second != first.Predecessor)));
        }

        // Draw the walls that don't cross a predecessor link.
        public void DrawWalls(Graphics gr, Pen pen)
        {
            for (int side = 0; side < 4; side++)
            {
                if (AreAdjacent(this, Neighbors[side], side))
                {
                    DrawWall(gr, pen, side, 0);
                }
            }
        }

        // Draw one side of our bounding box.
        private void DrawWall(Graphics gr, Pen pen, int side, int offset)
        {
            switch (side)
            {
                case North:
                    gr.DrawLine(pen,
                        Bounds.Left + offset, Bounds.Top + offset,
                        Bounds.Right - offset, Bounds.Top + offset);
                    break;
                case South:
                    gr.DrawLine(pen,
                        Bounds.Left + offset, Bounds.Bottom - offset,
                        Bounds.Right - offset, Bounds.Bottom - offset);
                    break;
                case East:
                    gr.DrawLine(pen,
                        Bounds.Right - offset, Bounds.Top + offset,
                        Bounds.Right - offset, Bounds.Bottom - offset);
                    break;
                case West:
                    gr.DrawLine(pen,
                        Bounds.Left + offset, Bounds.Top + offset,
                        Bounds.Left + offset, Bounds.Bottom - offset);
                    break;
            }
        }
    }
}
