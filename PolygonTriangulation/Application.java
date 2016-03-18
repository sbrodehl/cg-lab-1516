package PolygonTriangulation;

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
import java.util.HashMap;
import java.util.HashSet;
import java.util.Vector;

// Die Klasse MyPanel steht für die Zeichenfläche. Sie erfüllt die drei Inferfaces zu den Mausinteraktionen
class MyPanel extends JPanel implements MouseListener, MouseMotionListener, MouseWheelListener {
	private static final long serialVersionUID = 1L;
	
	// Der aktuelle Algorithmus
	Drawable2D alg;
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
		g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		
		// Definition des Weltkoordinatensystems, so dass der Ursprung in der Mitte des 
		// Zeichenfeldes liegt und das Einheitsquadrat sichtbar ist.
		g2.translate(w/2.0,h/2.0);
		if (w > h) g2.scale(h/2.0,-h/2.0);
		else g2.scale(w/2.0,-w/2.0);

		// Verkleinerung bzw. Vergrößerung gesteuert durch das Mausrad
		g2.scale(zoom,zoom);

		// Zeichne Ergebnis, falls vorhanden
		if(alg != null){
			alg.paint(g2);
		}
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
}

// Die Hauptklasse kann sowohl als Java-Applet als auch als Java-Applikation gestartet werden.
public class Application extends JApplet implements ActionListener {
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
		it1 = new JMenuItem("Triangulation Algorithm");

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

	// Wenn ein Menueintrag ausgewählt wurde, wird diese Methode aufgerufen.
	public void actionPerformed(ActionEvent e) {
		if (e.getSource().equals(it1)) {
			panel.alg = new TriangulationAlgorithm();
			Vector<Vector<Point2D>> points = new Vector<Vector<Point2D>>();
			points.add(new Vector<Point2D>());
			points.add(new Vector<Point2D>());
			
			points.get(0).add(new Point2D.Double(0.8, 0.3));
			points.get(0).add(new Point2D.Double(0.5, 0.2));
			points.get(0).add(new Point2D.Double(0.4, 0.8));
			points.get(0).add(new Point2D.Double(0.0, 0.6));
			points.get(0).add(new Point2D.Double(-0.4, 0.8));
			points.get(0).add(new Point2D.Double(-0.7, 0.5));
			points.get(0).add(new Point2D.Double(-0.5, 0.2));
			points.get(0).add(new Point2D.Double(-0.6, -0.2));
			points.get(0).add(new Point2D.Double(-0.8, 0.0));
			points.get(0).add(new Point2D.Double(-0.9, -0.4));
			points.get(0).add(new Point2D.Double(-0.6, -0.6));
			points.get(0).add(new Point2D.Double(-0.1, -0.4));
			points.get(0).add(new Point2D.Double(0.3, -0.9));
			points.get(0).add(new Point2D.Double(0.2, -0.1));
			points.get(0).add(new Point2D.Double(0.5, -0.2));
			
			points.get(1).add(new Point2D.Double(0.0, -0.3));
			points.get(1).add(new Point2D.Double(-0.3, -0.2));
			points.get(1).add(new Point2D.Double(-0.2, 0.3));
			points.get(1).add(new Point2D.Double(-0.1, 0.1));
			
			panel.alg.run(points);
		}
			
		if (e.getSource().equals(it2)) 
			System.exit(0);

		panel.repaint();
	}

	public static void main(String args[]) {	
		isApplet = false;
		JApplet applet = new Application();
		applet.setPreferredSize(new Dimension(500,500));
		applet.init();

		JFrame frame = new JFrame("Algorithmic Geometry");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setLayout(new BorderLayout());

		frame.add(applet,BorderLayout.CENTER);
		frame.setPreferredSize(new Dimension(500,500));
		frame.pack();
		frame.setVisible(true);	
	}
}