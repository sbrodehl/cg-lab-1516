import javax.swing.JApplet;
import javax.swing.JPanel;
import javax.swing.JFrame;
import javax.swing.JMenuBar;
import javax.swing.JMenu;
import javax.swing.JMenuItem;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics2D;
import java.awt.BasicStroke;
import java.awt.Graphics;
import java.awt.RenderingHints;
import java.awt.BorderLayout;
import java.awt.geom.*;
import java.awt.event.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Vector;

// Die Klasse MyPanel steht für die Zeichenfläche. Sie erfüllt die drei Inferfaces zu den Mausinteraktionen
class MyPanel extends JPanel implements MouseListener, MouseMotionListener, MouseWheelListener {
	private static final long serialVersionUID = 1L;

	// Die Eingabepunkte
	Vector<Point2D> points;
	// Die zu berechnenden Liniensegmente der Delaunay Triangulation
	Vector<Line2D> lines;
	// Die berechneten Dreiecke der Delaunay Triangulation
	Vector<Triangle2D> T;
	// Der Vergrößerungsfaktor kann über das Mausrad verändert werden.
	double zoom = 0.8;
	// Der Ort, an dem eine Maustaste gedrückt wurde (in Weltkoordinaten).
	Point2D pickPoint = new Point2D.Double();
	// Der letzte Ort, an den die Maus bewegt wurde (in Weltkoordinaten).
	Point2D lastMousePos = new Point2D.Double();

	// Konstruktor für das Zeichenfeld
	public MyPanel() {
		// Die MausInteraktionen müssen registiert werden, damit ein 
		// Ereignis in der Ereignisschleife ausgelöst wird.
		addMouseListener(this);
		addMouseMotionListener(this);
		addMouseWheelListener(this);
		
		// Anlegen eines leeren dynamischen Feldes für die Punkte
		points = new Vector<Point2D>();
		// und die Liniensegment der Delaunay Triangulation
		lines = new Vector<Line2D>();
		// und die Dreiecke der Delaunay Triangulation
		T = new Vector<Triangle2D>();
	}
	
	// Das geometrische Prädikat zur Überprüfung, ob ein Punkt c links der orientierten Geraden von a nach b liegt
	boolean leftTurn(Point2D a,Point2D b,Point2D c) {
		double ux = b.getX()-a.getX();
		double uy = b.getY()-a.getY();
		double vx = c.getX()-a.getX();
		double vy = c.getY()-a.getY();
		return (ux*vy-uy*vx > 0);
	}
	
	// Das geometrische Prädikat zur Überprüfung, ob ein Punkt d innerhalb des Umkreises von a, b und c liegt
	boolean inCircle(Point2D a, Point2D b, Point2D c, Point2D d){
		double ax = a.getX();
		double ay = a.getY();
		double as = ax*ax+ay*ay;
		
		double bx = b.getX();
		double by = b.getY();
		double bs = bx*bx+by*by;
		
		double cx = c.getX();
		double cy = c.getY();
		double cs = cx*cx+cy*cy;
		
		double dx = d.getX();
		double dy = d.getY();
		double ds = dx*dx+dy*dy;
		
		double bcd = (bx*cy*ds+cx*dy*bs+dx*by*cs)-(bs*cy*dx+cs*dy*bx+ds*by*cx);
		double acd = (ax*cy*ds+cx*dy*as+dx*ay*cs)-(as*cy*dx+cs*dy*ax+ds*ay*cx);
		double abd = (ax*by*ds+bx*dy*as+dx*ay*bs)-(as*by*dx+bs*dy*ax+ds*ay*bx);
		double abc = (ax*by*cs+bx*cy*as+cx*ay*bs)-(as*by*cx+bs*cy*ax+cs*ay*bx);
		
		return (bcd-acd+abd-abc < 0);
	}

	// Das Zeichen der Szene wird immer dann initiiert, wenn die Zeichenfläche sichtbar wird
	// (z.B. automatisch nach einer Verdeckung durch ein anderes Fenster) oder (manuell) durch
	// Aufruf der Methode repaint, wenn eine Änderung in der Szene stattgefunden hat.
	public void paint(Graphics g) {
		// Grafikkontext
		Graphics2D g2 = (Graphics2D) g;

		// Höhe und Breite des Zeichenfeldes
		int w = getWidth();
		int h = getHeight();

		// Löschen des Zeichenfeldes in der Farbe Weiß
		g2.setColor(Color.white);
		g2.fillRect(0,0,w,h);

		// Einschalten von Antialiasing zur Unterdrückung von Rasterisierungsartefakten
		g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
		
		// Definition des Weltkoordinatensystems, so dass der Ursprung in der Mitte des 
		// Zeichenfeldes liegt und das Einheitsquadrat sichtbar ist.
		g2.translate(w/2.0,h/2.0);
		if (w > h) g2.scale(h/2.0,-h/2.0);
		else g2.scale(w/2.0,-w/2.0);

		// Verkleinerung bzw. Vergrößerung gesteuert durch das Mausrad
		g2.scale(zoom,zoom);

		// Dicke des Zeichenstiftes für die Konturen
		g2.setStroke(new BasicStroke(0.01f));

		// Iteration Über alle vorhandenen Punkte
		g2.setColor(Color.black);
		double rad = 0.02;
		for(Point2D p: points)
			g2.fill(new Ellipse2D.Double(p.getX()-rad,p.getY()-rad,2*rad,2*rad));
		
		// Zeichnen der Linien
		lines.clear();
		for(Triangle2D t:T){
			Line2D[] ls = t.getLines();
			lines.add(ls[0]);
			lines.add(ls[1]);
			lines.add(ls[2]);
		}
		for(Line2D l: lines)
			g2.draw(l);
	}
	
	public void mouseClicked(MouseEvent event) {}

	public void mouseEntered(MouseEvent event) {}

	public void mouseExited(MouseEvent event) {}

	// Sobald eine Maustaste gedrückt wurde, ermitteln wir den Ort in Weltkoordinaten
	public void mousePressed(MouseEvent event) {
		Point2D p = worldCoord(event.getX(),event.getY());
		lastMousePos.setLocation(p);
		
		// repaint();
	}

	public void mouseReleased(MouseEvent event) {
		// repaint();
	}

	public void mouseDragged(MouseEvent event) {
		Point2D p = worldCoord(event.getX(),event.getY());
		// double tx = p.getX()-lastMousePos.getX();
		// double ty = p.getY()-lastMousePos.getY();

		lastMousePos.setLocation(p);
		repaint();    
	}

	public void mouseMoved(MouseEvent event) {}

	// Wenn das Mausrad gedreht wurde, soll sich der Vergrößerungsfaktor der Szene
	// entsprechend Ändern.
	public void mouseWheelMoved(MouseWheelEvent event) {
		if (event.getWheelRotation() < 0) zoom *= 1.1; else zoom /= 1.1;
        repaint();        
	}
	
	// Manuelle Umrechnung der Bildschirmkoordinaten aus [0,w-1]x[0,h-1] in die
	// "normalisierten" Weltkoordinaten.
	private Point2D worldCoord(int x,int y) {
		double w = getWidth();
		double h = getHeight();
		double px =  2.0*x/h-w/h;
		double py = -2.0*y/h+1.0;
	    px /= zoom;
	    py /= zoom;
	    
	    return new Point2D.Double(px,py);
	}

	// Dreieck-Klasse für die Delaunay Triangulation
	public class Triangle2D implements Comparable<Triangle2D>{
		private Point2D[] points = new Point2D[3];
		private Line2D[] lines = new Line2D[3];
		private Triangle2D[] neighbors = new Triangle2D[3];
		
		public Triangle2D(Point2D a, Point2D b, Point2D c){
			
			if(leftTurn(a, b, c)){
				points[0] = a;
				points[1] = b;
				points[2] = c;
				
				lines[0] = new Line2D.Double(a, b);
				lines[1] = new Line2D.Double(b, c);
				lines[2] = new Line2D.Double(c, a);
			}else{
				points[0] = a;
				points[1] = c;
				points[2] = b;
				
				lines[0] = new Line2D.Double(a, c);
				lines[1] = new Line2D.Double(c, b);
				lines[2] = new Line2D.Double(b, a);
			}
		}
		
		public void setNeighbor(int i, Triangle2D t){
			neighbors[i] = t;
		}
		
		public Triangle2D getNeighbor(int i){
			return neighbors[i];
		}
		
		public Point2D[] getPoints(){
			return points;
		}
		
		public Line2D[] getLines(){
			return lines;
		}
		
		public boolean contains(Point2D p){
			for(int i=0;i<3;i++){
				if(p.equals(points[i])){
					return true;
				}
			}
			return false;
		}
		
		public boolean contains(Line2D l){
			return contains(l.getP1()) && contains(l.getP2());
		}
		
		public int indexOf(Point2D p){
			for(int i=0;i<3;i++){
				if(p.equals(points[i])){
					return i;
				}
			}
			return -1;
		}
		
		public int indexOf(Line2D l){
			for(int i=0;i<3;i++){
				if(l.getP1().equals(lines[i].getP1()) && l.getP2().equals(lines[i].getP2())){
					return i;
				}
				if(l.getP2().equals(lines[i].getP1()) && l.getP1().equals(lines[i].getP2())){
					return i;
				}
			}
			return -1;
		}
		
		public Point2D getOppositePoint(Line2D l){
			if(!contains(l)){
				return null;
			}
			for(int i=0;i<3;i++){
				Point2D p = points[i];
				if(!p.equals(l.getP1()) && !p.equals(l.getP2())){
					return p;
				}
			}
			return null;
		}
		
		public boolean includes(Point2D p){
			for(int i=0; i<3; i++){
				Line2D l = lines[i];
				if(!leftTurn(l.getP1(), l.getP2(), p)){
					return false;
				}
			}
			return true;
		}

		@Override
		public int compareTo(Triangle2D t) {
			Point2D[] pts = t.getPoints();
			if(!contains(pts[0])){
				return -1;
			}
			if(!contains(pts[1])){
				return -1;
			}
			if(!contains(pts[2])){
				return -1;
			}
			return 0;
		}
		
		public boolean equals(Triangle2D t){
			return compareTo(t) == 0;
		}
		
		public String toString(){
			return "Triangle: "+points[0]+" - "+points[1]+" - "+points[2];
		}
	}
	
	//Triangulations-Methode
	public void triangulate() {
		boolean debug = false;
		
		if(points.size() < 6){
			return;
		}
		
		T = new Vector<Triangle2D>();
		Triangle2D t0 = new Triangle2D(points.get(0), points.get(1), points.get(2));
		T.add(t0);
		
		for(int i=3; i<points.size(); i++){
			if(debug)
				System.out.println("Step "+(i-2));
			Point2D a = points.get(i);
			if(debug)
				System.out.println("New Point : "+a);
			
			// Check if point is inside an Triangle
			Triangle2D ta = null;
			for(Triangle2D t : T){
				if(t.includes(a)){
					ta = t;
				}
			}
			if(debug)
				System.out.println("Triangle "+ta+" includes "+a);
			// Create new Triangles, Update T and lines
			Line2D[] ls = ta.getLines();
			Triangle2D ta0 = new Triangle2D(ls[0].getP1(), ls[0].getP2(), a);
			Triangle2D ta1 = new Triangle2D(ls[1].getP1(), ls[1].getP2(), a);
			Triangle2D ta2 = new Triangle2D(ls[2].getP1(), ls[2].getP2(), a);
			
			T.add(ta0);
			T.add(ta1);
			T.add(ta2);
			T.remove(ta);
			
			// Add neighborhood
			ta0.setNeighbor(0, ta.getNeighbor(0));
			ta0.setNeighbor(1, ta1);
			ta0.setNeighbor(2, ta2);
			
			ta1.setNeighbor(0, ta.getNeighbor(1));
			ta1.setNeighbor(1, ta2);
			ta1.setNeighbor(2, ta0);
			
			ta2.setNeighbor(0, ta.getNeighbor(2));
			ta2.setNeighbor(1, ta0);
			ta2.setNeighbor(2, ta1);
			
			// Update neighborhood for neighbors of ta
			if(ta.getNeighbor(0) != null){
				Triangle2D n0 = ta.getNeighbor(0);
				int index = n0.indexOf(ta0.getLines()[0]);
				n0.setNeighbor(index, ta0);
			}
			
			if(ta.getNeighbor(1) != null){
				Triangle2D n1 = ta.getNeighbor(1);
				int index = n1.indexOf(ta1.getLines()[0]);
				n1.setNeighbor(index, ta1);
			}
			
			if(ta.getNeighbor(2) != null){
				Triangle2D n2 = ta.getNeighbor(2);
				int index = n2.indexOf(ta2.getLines()[0]);
				n2.setNeighbor(index, ta2);
			}
			
			// Check InCircle predicate, flip edges if necessary
			Queue<Triangle2D> queue = new LinkedList<Triangle2D>();
			queue.add(ta0);
			queue.add(ta1);
			queue.add(ta2);
			
			while(!queue.isEmpty()){
				Triangle2D next = queue.poll();
				if(debug)
					System.out.println("Testing "+next);
				
				Triangle2D n0 = next.getNeighbor(0);
				Triangle2D n1 = next.getNeighbor(1);
				Triangle2D n2 = next.getNeighbor(2);
				
				if(n0 != null){
					if(debug)
						System.out.println("Testing neighbor"+n0);
					Point2D d = n0.getOppositePoint(next.getLines()[0]);
					if(inCircle(next.getPoints()[0], next.getPoints()[1], next.getPoints()[2], d) && leftTurn(next.getLines()[2].getP1(), next.getLines()[2].getP2(), d) && leftTurn(d, next.getLines()[1].getP1(), next.getLines()[1].getP2())){
						// Edge Flip
						if(debug)
							System.out.println("Flipping Edge: "+next.getLines()[0].getP1()+" - "+next.getLines()[0].getP2());
						
						T.remove(next);
						T.remove(n0);
						
						queue.remove(n0);
						
						Triangle2D newNext = new Triangle2D(next.getOppositePoint(next.getLines()[0]), next.getLines()[0].getP1(), n0.getOppositePoint(next.getLines()[0]));
						Triangle2D newN0 = new Triangle2D(next.getOppositePoint(next.getLines()[0]), n0.getOppositePoint(next.getLines()[0]), next.getLines()[0].getP2());
						
						// Update neighborhood
						
						// Neighborhood of newNext and newN0
						Line2D newLine = new Line2D.Double(next.getOppositePoint(next.getLines()[0]), n0.getOppositePoint(next.getLines()[0]));
						newNext.setNeighbor(newNext.indexOf(newLine), newN0);
						newN0.setNeighbor(newN0.indexOf(newLine), newNext);
						
						// Other neighbors of newNext
						Line2D newNextLine1 = new Line2D.Double(next.getOppositePoint(next.getLines()[0]), next.getLines()[0].getP1());
						Line2D newNextLine2 = new Line2D.Double(next.getLines()[0].getP1(), n0.getOppositePoint(next.getLines()[0]));
						
						if(next.contains(newNextLine1)){
							Triangle2D neighbor = next.getNeighbor(next.indexOf(newNextLine1));
							if(neighbor != null){
								newNext.setNeighbor(newNext.indexOf(newNextLine1), neighbor);
								neighbor.setNeighbor(neighbor.indexOf(newNextLine1), newNext);
							}
						}else{
							Triangle2D neighbor = n0.getNeighbor(n0.indexOf(newNextLine1));
							if(neighbor != null){
								newNext.setNeighbor(newNext.indexOf(newNextLine1), neighbor);
								neighbor.setNeighbor(neighbor.indexOf(newNextLine1), newNext);
							}
						}
						
						if(next.contains(newNextLine2)){
							Triangle2D neighbor = next.getNeighbor(next.indexOf(newNextLine2));
							if(neighbor != null){
								newNext.setNeighbor(newNext.indexOf(newNextLine2), neighbor);
								neighbor.setNeighbor(neighbor.indexOf(newNextLine2), newNext);
							}
						}else{
							Triangle2D neighbor = n0.getNeighbor(n0.indexOf(newNextLine2));
							if(neighbor != null){
								newNext.setNeighbor(newNext.indexOf(newNextLine2), neighbor);
								neighbor.setNeighbor(neighbor.indexOf(newNextLine2), newNext);
							}
						}
						
						// Other neighbors of newN0
						Line2D newN0Line1 = new Line2D.Double(n0.getOppositePoint(next.getLines()[0]), next.getLines()[0].getP2());
						Line2D newN0Line2 = new Line2D.Double(next.getLines()[0].getP2(), next.getOppositePoint(next.getLines()[0]));
						
						if(next.contains(newN0Line1)){
							Triangle2D neighbor = next.getNeighbor(next.indexOf(newN0Line1));
							if(neighbor != null){
								newN0.setNeighbor(newN0.indexOf(newN0Line1), neighbor);
								neighbor.setNeighbor(neighbor.indexOf(newN0Line1), newN0);
							}
						}else{
							Triangle2D neighbor = n0.getNeighbor(n0.indexOf(newN0Line1));
							if(neighbor != null){
								newN0.setNeighbor(newN0.indexOf(newN0Line1), neighbor);
								neighbor.setNeighbor(neighbor.indexOf(newN0Line1), newN0);
							}
						}
						
						if(next.contains(newN0Line2)){
							Triangle2D neighbor = next.getNeighbor(next.indexOf(newN0Line2));
							if(neighbor != null){
								newN0.setNeighbor(newN0.indexOf(newN0Line2), neighbor);
								neighbor.setNeighbor(neighbor.indexOf(newN0Line2), newN0);
							}
						}else{
							Triangle2D neighbor = n0.getNeighbor(n0.indexOf(newN0Line2));
							if(neighbor != null){
								newN0.setNeighbor(newN0.indexOf(newN0Line2), neighbor);
								neighbor.setNeighbor(neighbor.indexOf(newN0Line2), newN0);
							}
						}
						
						T.add(newNext);
						T.add(newN0);
						
						queue.add(newNext);
						queue.add(newN0);
						
						continue;
					}
				}
				
				if(n1 != null){
					if(debug)
						System.out.println("Testing neighbor"+n1);
					Point2D d = n1.getOppositePoint(next.getLines()[1]);
					if(inCircle(next.getPoints()[0], next.getPoints()[1], next.getPoints()[2], d) && leftTurn(next.getLines()[0].getP1(), next.getLines()[0].getP2(), d) && leftTurn(d, next.getLines()[2].getP1(), next.getLines()[2].getP2())){
						// Edge Flip
						if(debug)
							System.out.println("Flipping Edge: "+next.getLines()[1].getP1()+" - "+next.getLines()[1].getP2());
						
						
						T.remove(next);
						T.remove(n1);
						
						queue.remove(n1);
						
						Triangle2D newNext = new Triangle2D(next.getOppositePoint(next.getLines()[1]), next.getLines()[1].getP1(), n1.getOppositePoint(next.getLines()[1]));
						Triangle2D newN1 = new Triangle2D(next.getOppositePoint(next.getLines()[1]), n1.getOppositePoint(next.getLines()[1]), next.getLines()[1].getP2());
						
						// Update neighborhood
						
						// Neighborhood of newNext and newN1
						Line2D newLine = new Line2D.Double(next.getOppositePoint(next.getLines()[1]), n1.getOppositePoint(next.getLines()[1]));
						newNext.setNeighbor(newNext.indexOf(newLine), newN1);
						newN1.setNeighbor(newN1.indexOf(newLine), newNext);
						
						// Other neighbors of newNext
						Line2D newNextLine1 = new Line2D.Double(next.getOppositePoint(next.getLines()[1]), next.getLines()[1].getP1());
						Line2D newNextLine2 = new Line2D.Double(next.getLines()[1].getP1(), n1.getOppositePoint(next.getLines()[1]));
						
						if(next.contains(newNextLine1)){
							Triangle2D neighbor = next.getNeighbor(next.indexOf(newNextLine1));
							if(neighbor != null){
								newNext.setNeighbor(newNext.indexOf(newNextLine1), neighbor);
								neighbor.setNeighbor(neighbor.indexOf(newNextLine1), newNext);
							}
						}else{
							Triangle2D neighbor = n1.getNeighbor(n1.indexOf(newNextLine1));
							if(neighbor != null){
								newNext.setNeighbor(newNext.indexOf(newNextLine1), neighbor);
								neighbor.setNeighbor(neighbor.indexOf(newNextLine1), newNext);
							}
						}
						
						if(next.contains(newNextLine2)){
							Triangle2D neighbor = next.getNeighbor(next.indexOf(newNextLine2));
							if(neighbor != null){
								newNext.setNeighbor(newNext.indexOf(newNextLine2), neighbor);
								neighbor.setNeighbor(neighbor.indexOf(newNextLine2), newNext);
							}
						}else{
							Triangle2D neighbor = n1.getNeighbor(n1.indexOf(newNextLine2));
							if(neighbor != null){
								newNext.setNeighbor(newNext.indexOf(newNextLine2), neighbor);
								neighbor.setNeighbor(neighbor.indexOf(newNextLine2), newNext);
							}
						}
						
						// Other neighbors of newN1
						Line2D newN1Line1 = new Line2D.Double(n1.getOppositePoint(next.getLines()[1]), next.getLines()[1].getP2());
						Line2D newN1Line2 = new Line2D.Double(next.getLines()[1].getP2(), next.getOppositePoint(next.getLines()[1]));
						
						if(next.contains(newN1Line1)){
							Triangle2D neighbor = next.getNeighbor(next.indexOf(newN1Line1));
							if(neighbor != null){
								newN1.setNeighbor(newN1.indexOf(newN1Line1), neighbor);
								neighbor.setNeighbor(neighbor.indexOf(newN1Line1), newN1);
							}
						}else{
							Triangle2D neighbor = n1.getNeighbor(n1.indexOf(newN1Line1));
							if(neighbor != null){
								newN1.setNeighbor(newN1.indexOf(newN1Line1), neighbor);
								neighbor.setNeighbor(neighbor.indexOf(newN1Line1), newN1);
							}
						}
						
						if(next.contains(newN1Line2)){
							Triangle2D neighbor = next.getNeighbor(next.indexOf(newN1Line2));
							if(neighbor != null){
								newN1.setNeighbor(newN1.indexOf(newN1Line2), neighbor);
								neighbor.setNeighbor(neighbor.indexOf(newN1Line2), newN1);
							}
						}else{
							Triangle2D neighbor = n1.getNeighbor(n1.indexOf(newN1Line2));
							if(neighbor != null){
								newN1.setNeighbor(newN1.indexOf(newN1Line2), neighbor);
								neighbor.setNeighbor(neighbor.indexOf(newN1Line2), newN1);
							}
						}
						
						T.add(newNext);
						T.add(newN1);
						
						queue.add(newNext);
						queue.add(newN1);
						
						continue;
					}
				}
				
				if(n2 != null){
					if(debug)
						System.out.println("Testing neighbor"+n2);
					Point2D d = n2.getOppositePoint(next.getLines()[2]);
					if(inCircle(next.getPoints()[0], next.getPoints()[1], next.getPoints()[2], d) && leftTurn(next.getLines()[1].getP1(), next.getLines()[1].getP2(), d) && leftTurn(d, next.getLines()[0].getP1(), next.getLines()[0].getP2())){
						// Edge Flip
						if(debug)
							System.out.println("Flipping Edge: "+next.getLines()[2].getP1()+" - "+next.getLines()[2].getP2());
						
						
						T.remove(next);
						T.remove(n2);
						
						queue.remove(n2);
						
						Triangle2D newNext = new Triangle2D(next.getOppositePoint(next.getLines()[2]), next.getLines()[2].getP1(), n2.getOppositePoint(next.getLines()[2]));
						Triangle2D newN2 = new Triangle2D(next.getOppositePoint(next.getLines()[2]), n2.getOppositePoint(next.getLines()[2]), next.getLines()[2].getP2());
						
						// Update neighborhood
						
						// Neighborhood of newNext and newN2
						Line2D newLine = new Line2D.Double(next.getOppositePoint(next.getLines()[2]), n2.getOppositePoint(next.getLines()[2]));
						newNext.setNeighbor(newNext.indexOf(newLine), newN2);
						newN2.setNeighbor(newN2.indexOf(newLine), newNext);
						
						// Other neighbors of newNext
						Line2D newNextLine1 = new Line2D.Double(next.getOppositePoint(next.getLines()[2]), next.getLines()[2].getP1());
						Line2D newNextLine2 = new Line2D.Double(next.getLines()[2].getP1(), n2.getOppositePoint(next.getLines()[2]));
						
						if(next.contains(newNextLine1)){
							Triangle2D neighbor = next.getNeighbor(next.indexOf(newNextLine1));
							if(neighbor != null){
								newNext.setNeighbor(newNext.indexOf(newNextLine1), neighbor);
								neighbor.setNeighbor(neighbor.indexOf(newNextLine1), newNext);
							}
						}else{
							Triangle2D neighbor = n2.getNeighbor(n2.indexOf(newNextLine1));
							if(neighbor != null){
								newNext.setNeighbor(newNext.indexOf(newNextLine1), neighbor);
								neighbor.setNeighbor(neighbor.indexOf(newNextLine1), newNext);
							}
						}
						
						if(next.contains(newNextLine2)){
							Triangle2D neighbor = next.getNeighbor(next.indexOf(newNextLine2));
							if(neighbor != null){
								newNext.setNeighbor(newNext.indexOf(newNextLine2), neighbor);
								neighbor.setNeighbor(neighbor.indexOf(newNextLine2), newNext);
							}
						}else{
							Triangle2D neighbor = n2.getNeighbor(n2.indexOf(newNextLine2));
							if(neighbor != null){
								newNext.setNeighbor(newNext.indexOf(newNextLine2), neighbor);
								neighbor.setNeighbor(neighbor.indexOf(newNextLine2), newNext);
							}
						}
						
						// Other neighbors of newN2
						Line2D newN2Line1 = new Line2D.Double(n2.getOppositePoint(next.getLines()[2]), next.getLines()[2].getP2());
						Line2D newN2Line2 = new Line2D.Double(next.getLines()[2].getP2(), next.getOppositePoint(next.getLines()[2]));
						
						if(next.contains(newN2Line1)){
							Triangle2D neighbor = next.getNeighbor(next.indexOf(newN2Line1));
							if(neighbor != null){
								newN2.setNeighbor(newN2.indexOf(newN2Line1), neighbor);
								neighbor.setNeighbor(neighbor.indexOf(newN2Line1), newN2);
							}
						}else{
							Triangle2D neighbor = n2.getNeighbor(n2.indexOf(newN2Line1));
							if(neighbor != null){
								newN2.setNeighbor(newN2.indexOf(newN2Line1), neighbor);
								neighbor.setNeighbor(neighbor.indexOf(newN2Line1), newN2);
							}
						}
						
						if(next.contains(newN2Line2)){
							Triangle2D neighbor = next.getNeighbor(next.indexOf(newN2Line2));
							if(neighbor != null){
								newN2.setNeighbor(newN2.indexOf(newN2Line2), neighbor);
								neighbor.setNeighbor(neighbor.indexOf(newN2Line2), newN2);
							}
						}else{
							Triangle2D neighbor = n2.getNeighbor(n2.indexOf(newN2Line2));
							if(neighbor != null){
								newN2.setNeighbor(newN2.indexOf(newN2Line2), neighbor);
								neighbor.setNeighbor(neighbor.indexOf(newN2Line2), newN2);
							}
						}
						
						T.add(newNext);
						T.add(newN2);
						
						queue.add(newNext);
						queue.add(newN2);
						
						continue;
					}
				}
			}
		}
		// remove initial points
		Point2D p1 = new Point2D.Double(-3, -1);
		Point2D p2 = new Point2D.Double(3, -1);
		Point2D p3 = new Point2D.Double(0, 2);
		
		Vector<Triangle2D> Tc = (Vector<Triangle2D>) T.clone();
		
		for(Triangle2D t : Tc){
			if(t.contains(p1)){
				T.remove(t);
			}
			if(t.contains(p2)){
				T.remove(t);
			}
			if(t.contains(p3)){
				T.remove(t);
			}
		}
		
		// Overall Test
		for(Triangle2D t : T){
			Triangle2D n0 = t.getNeighbor(0);
			Triangle2D n1 = t.getNeighbor(1);
			Triangle2D n2 = t.getNeighbor(2);
			
			// System.out.println("Testing "+t);
			
			if(n0 != null){
				Point2D d = n0.getOppositePoint(t.getLines()[0]);
				if(inCircle(t.getPoints()[0], t.getPoints()[1], t.getPoints()[2], d)){
					System.err.println("InCircle Error!");
				}
			}
			if(n1 != null){
				Point2D d = n1.getOppositePoint(t.getLines()[1]);
				if(inCircle(t.getPoints()[0], t.getPoints()[1], t.getPoints()[2], d)){
					System.err.println("InCircle Error!");
				}
			}
			if(n2 != null){
				Point2D d = n2.getOppositePoint(t.getLines()[2]);
				if(inCircle(t.getPoints()[0], t.getPoints()[1], t.getPoints()[2], d)){
					System.err.println("InCircle Error!");
				}
			}
		}
	}
}

// Die Hauptklasse kann sowohl als Java-Applet als auch als Java-Applikation gestartet werden.
public class DelaunayTriangulationIncremental extends JApplet implements ActionListener {
	private static final long serialVersionUID = 1L;
	static boolean isApplet = true;

	private JMenuBar mb;
	JMenu    m1;
	JMenuItem it1,it2;
	MyPanel panel;

	// Die grafische BenutzeroberflÃ¤che besteht aus einer Menuleiste und unserem Zeichenfeld
	public void init() {
		mb = new JMenuBar();
		m1 = new JMenu("Actions");
		it1 = new JMenuItem("Random points");

		it1.addActionListener(this);

		m1.add(it1);

		if (!isApplet) {
			it2 = new JMenuItem("Exit");
			it2.addActionListener(this);
			m1.addSeparator();
			m1.add(it2);
		}

		mb.add(m1);

		add("North",mb);
		panel = new MyPanel();
		add(panel);
	}

	// Wenn ein Menueintrag ausgewÃ¤hlt wurde, wird diese Methode aufgerufen.
	public void actionPerformed(ActionEvent e) {
		if (e.getSource().equals(it1)) {
			panel.points.clear();
			panel.lines.clear();
			// Erzeugen einer zufÃ¤lligen Probleminstanz
			
			panel.points.add(new Point2D.Double(-3,-1));
			panel.points.add(new Point2D.Double(3,-1));
			panel.points.add(new Point2D.Double(0,2));
			
			for(int i=0;i<25;i++)
				panel.points.add(new Point2D.Double(2*Math.random()-1,2*Math.random()-1));
			
			panel.triangulate();
		}
			
		if (e.getSource().equals(it2)) 
			System.exit(0);

		panel.repaint();
	}

	public static void main(String args[]) {	
		isApplet = false;
		JApplet applet = new DelaunayTriangulationIncremental();
		applet.setPreferredSize(new Dimension(500,500));
		applet.init();

		JFrame frame = new JFrame("Delaunay Triangulation Incremental");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setLayout(new BorderLayout());

		frame.add(applet,BorderLayout.CENTER);
		frame.setPreferredSize(new Dimension(500,500));
		frame.pack();
		frame.setVisible(true);	
	}
}