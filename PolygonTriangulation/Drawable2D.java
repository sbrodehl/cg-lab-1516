package PolygonTriangulation;

import java.awt.Graphics2D;
import java.awt.geom.Point2D;
import java.util.Vector;

public interface Drawable2D {
	public void paint(Graphics2D g);
	public void run(Vector<Vector<Point2D>> points);
}
