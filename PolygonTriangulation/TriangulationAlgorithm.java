package PolygonTriangulation;

import java.awt.*;
import java.awt.geom.*;
import java.util.ArrayDeque;
import java.util.Comparator;
import java.util.HashMap;
import java.util.PriorityQueue;
import java.util.TreeSet;
import java.util.Vector;

import PolygonTriangulation.Vertex.Type;

public class TriangulationAlgorithm implements Drawable2D {
	Vector<Point2D> points;
	Vector<Line2D> lines;
	Vector<Vertex> vertices;
	HashMap<Vertex, Vector<Edge>> graph;
	Vector<Vector<Vertex>> polygons;
	
	public TriangulationAlgorithm(){
		lines = new Vector<Line2D>();
		vertices = new Vector<Vertex>();
		points = new Vector<Point2D>();
		graph = new HashMap<Vertex, Vector<Edge>>();
		polygons = new Vector<Vector<Vertex>>();
	}
	
	@Override
	public void paint(Graphics2D g) {
		// Dicke des Zeichenstiftes f�r die Konturen
		g.setStroke(new BasicStroke(0.01f));
		g.setColor(Color.black);
		
		// Iteration �ber alle vorhandenen Punkte
		double rad = 0.02;
		for(Vertex v: vertices){
			if(v.getType() == Type.START){
				g.setColor(Color.yellow);
			}
			if(v.getType() == Type.SPLIT){
				g.setColor(Color.green);
			}
			if(v.getType() == Type.END){
				g.setColor(Color.red);
			}
			if(v.getType() == Type.MERGE){
				g.setColor(Color.blue);
			}
			if(v.getType() == Type.REGULAR){
				g.setColor(Color.black);
			}
			g.fill(new Ellipse2D.Double(v.getX()-rad,v.getY()-rad,2*rad,2*rad));
		}
		
		g.setColor(Color.black);
		
		// Iteration �ber alle Linien
		for(Line2D line : lines){
			g.draw(line);
		}
	}

	@Override
	public void run(Vector<Vector<Point2D>> points) {
		for(Vector<Point2D> pointVector : points){
			this.points.addAll(pointVector);
			for(int i=0;i<pointVector.size();i++){
				lines.add(new Line2D.Double(pointVector.get(i), pointVector.get((i+1)%pointVector.size())));
				vertices.add(new Vertex(pointVector.get(i)));
			}
		}
		
		buildVertices();
		makeMonotone();
		createPolygons();
		for(Vector<Vertex> polygon : polygons){
			triangulate(polygon);
		}
	}
	
	private void triangulate(Vector<Vertex> polygon){
		PriorityQueue<Vertex> pq = new PriorityQueue<Vertex>(new Comparator<Vertex>(){
			@Override
			public int compare(Vertex a, Vertex b) {
				if(a.getY() < b.getY()){
					return 1;
				}
				if(a.getY() > b.getY()){
					return -1;
				}
				if(a.getX() > b.getX()){
					return 1;
				}
				if(a.getX() < b.getX()){
					return -1;
				}
				return 0;
			}
			
		});
		
		pq.addAll(polygon);
		
		// get min
		Vertex min = polygon.get(0);
		double min_val = polygon.get(0).getY();
		
		for(Vertex v : polygon){
			if(v.getY() < min_val){
				min_val = v.getY();
				min = v;
			}
			if(v.getY() == min_val && v.getX() > min.getX()){
				min_val = v.getY();
				min = v;
			}
		}
		
		// create paths
		Vector<Vertex> left = new Vector<Vertex>();
		Vector<Vertex> right = new Vector<Vertex>();
		
		Vertex max = pq.peek();
		Vertex actual = polygon.get((polygon.indexOf(max)+1) % polygon.size());;
		while(actual != min){
			left.add(actual);
			actual = polygon.get((polygon.indexOf(actual)+1) % polygon.size());
		}
		
		actual = polygon.get((polygon.indexOf(actual)+1) % polygon.size());;
		while(actual != max){
			right.add(actual);
			actual = polygon.get((polygon.indexOf(actual)+1) % polygon.size());
		}
		
		// print polygon
		System.out.println("Polygon");
		for(Vertex v : polygon){
			System.out.print(v+" - ");
			System.out.println();
		}
		
		System.out.println("Max : "+max+ ", Min : "+min);
		
		// print left
		System.out.println("left:");
		for(Vertex v : left){
			System.out.print(v+" - ");
			System.out.println();
		}
		
		// print right
		System.out.println("right:");
		for(Vertex v : right){
			System.out.print(v+" - ");
			System.out.println();
		}
		
		System.out.println("---------------------------------------------------------");
		
		ArrayDeque<Vertex> stack = new ArrayDeque<Vertex>();
		
		stack.push(pq.poll());
		stack.push(pq.poll());
		
		Vertex v_last = stack.peek();
		
		while(pq.size() > 1){
			Vertex v = pq.poll();
			Vertex top = stack.peek();
			
			if(( left.contains(v) && right.contains(top) ) || ( right.contains(v) && left.contains(top) )){
				while(!stack.isEmpty()){
					Vertex next = stack.pop();
					if(!stack.isEmpty()){
						lines.add(new Line2D.Double(v, next));
					}
				}
				stack.push(v_last);
				stack.push(v);
			}else{
				Vertex next = stack.pop();
				System.out.println("same path "+v+" - "+next);
				while(!stack.isEmpty() && canSee(v, next, stack.peek(), left)){
					System.out.println("can see, draw line");
					System.out.println(v+" -> "+stack.peek());
					next = stack.pop();
					lines.add(new Line2D.Double(v, next));
				}
				stack.push(next);
				stack.push(v);
			}
			
			v_last = v;
		}
		
		Vertex v = pq.poll();

		stack.pop();
		while(!stack.isEmpty()){
			Vertex next = stack.pop();
			if(!stack.isEmpty()){
				lines.add(new Line2D.Double(v, next));
			}
		}
	}
	
	private boolean canSee(Vertex v, Vertex next, Vertex vertex, Vector<Vertex> left){
		if(left.contains(v)){
			return Vertex.leftTurn(next, v, vertex);
		}else{
			return Vertex.leftTurn(v, next, vertex);
		}
	}
	
	private void createPolygons(){
		for(Vector<Edge> ev : graph.values()){
			for(Edge e : ev){
				if(e.visited){
					continue;
				}
				
				Vector<Vertex> polygon = new Vector<Vertex>();
				Vertex start = e.v1();
				Edge actual = e;
				do{
					actual.visited = true;
					polygon.add(actual.v1());
					
					Vector<Edge> outEdges = graph.get(actual.v2());
					// get most left edge
					Edge out = outEdges.get(0);
					double min_angle = Vertex.angleBetween2Lines(actual.v1(), actual.v2(), out.v2());
					for(Edge o : outEdges){
						if(o.visited){
							continue;
						}
						if(actual.v1() == o.v2()){
							continue;
						}
						double angle = Vertex.angleBetween2Lines(actual.v1(), actual.v2(), o.v2());
						if(angle < min_angle){
							min_angle = angle;
							out = o;
						}
					}
					
					actual = out;
					
				}while(actual.v1() != start);
				polygons.add(polygon);
			}
		}
	}
	
	private void buildVertices(){
		for(Line2D line : lines){
			Vertex v1 = vertices.get(points.indexOf(line.getP1()));
			Vertex v2 = vertices.get(points.indexOf(line.getP2()));
			
			v1.setNeighbor(1, v2);
			v2.setNeighbor(0, v1);
			
			graph.put(v1, new Vector<Edge>());
			graph.get(v1).add(new Edge(v1, v2));
		}
		for(Vertex v : vertices){
			v.setType();
		}
	}
	
	private void makeMonotone(){
		PriorityQueue<Vertex> pq = new PriorityQueue<Vertex>(new Comparator<Vertex>(){			
			@Override
			public int compare(Vertex a, Vertex b) {
				if(a.getY() < b.getY()){
					return 1;
				}
				if(a.getY() > b.getY()){
					return -1;
				}
				if(a.getX() > b.getX()){
					return 1;
				}
				if(a.getX() < b.getX()){
					return -1;
				}
				return 0;
			}
			
		});
		
		pq.addAll(vertices);
		
		TreeSet<Line2D> SL = new TreeSet<Line2D>(new Comparator<Line2D>(){

			@Override
			public int compare(Line2D o1, Line2D o2) {
				if(o1 == o2){
					return 0;
				}
				Point2D p1 = o1.getY1() > o1.getY2() ? o1.getP1() : o1.getP2();
				Point2D p2 = o2.getY1() > o2.getY2() ? o2.getP1() : o2.getP2();
				
				if(p1.getX() > p2.getX()){
					return 1;
				}else{
					return -1;
				}
			}
			
		});
		
		HashMap<Line2D, Vertex> helpers = new HashMap<Line2D, Vertex>();
		
		while(!pq.isEmpty()){
			Vertex p = pq.poll();
			handleVertex(p, SL, helpers);
		}
	}
	
	private void handleVertex(Vertex p, TreeSet<Line2D> SL, HashMap<Line2D, Vertex> helpers){
		if(p.getType() == Type.START){
			handleStart(p, SL, helpers);
		}
		if(p.getType() == Type.SPLIT){
			handleSplit(p, SL, helpers);
		}
		if(p.getType() == Type.END){
			handleEnd(p, SL, helpers);
		}
		if(p.getType() == Type.MERGE){
			handleMerge(p, SL, helpers);
		}
		if(p.getType() == Type.REGULAR){
			handleRegular(p, SL, helpers);
		}
	}
	
	private void handleStart(Vertex p, TreeSet<Line2D> SL, HashMap<Line2D, Vertex> helpers){
		int idx = vertices.indexOf(p);
		SL.add(lines.get(idx));
		helpers.put(lines.get(idx), p);
	}
	
	private void handleEnd(Vertex p, TreeSet<Line2D> SL, HashMap<Line2D, Vertex> helpers){
		int idx = vertices.indexOf(p.getNeighbor(0));
		Line2D e = lines.get(idx);

		Vertex p2 = helpers.get(e);
		if(p2.getType() == Type.MERGE){
			lines.add(new Line2D.Double(p, p2));
			graph.get(p).add(new Edge(p, p2));
			graph.get(p2).add(new Edge(p2, p));
		}
		SL.remove(e);
	}
	
	private void handleSplit(Vertex p, TreeSet<Line2D> SL, HashMap<Line2D, Vertex> helpers){
		Line2D e = SL.lower(new Line2D.Double(p, p));
		Vertex p2 = helpers.get(e);
		lines.add(new Line2D.Double(p, p2));
		graph.get(p).add(new Edge(p, p2));
		graph.get(p2).add(new Edge(p2, p));
		
		helpers.put(e, p);
		
		int idx = vertices.indexOf(p);
		SL.add(lines.get(idx));
		helpers.put(lines.get(idx), p);
	}
	
	private void handleMerge(Vertex p, TreeSet<Line2D> SL, HashMap<Line2D, Vertex> helpers){
		int idx = vertices.indexOf(p.getNeighbor(0));
		Line2D e = lines.get(idx);
		
		Vertex p2 = helpers.get(e);
		if(p2.getType() == Type.MERGE){
			lines.add(new Line2D.Double(p, p2));
			graph.get(p).add(new Edge(p, p2));
			graph.get(p2).add(new Edge(p2, p));
		}
		SL.remove(e);
		
		Line2D e2 = SL.lower(new Line2D.Double(p, p));
		Vertex p3 = helpers.get(e2);
		
		if(p3.getType() == Type.MERGE){
			lines.add(new Line2D.Double(p, p3));
			graph.get(p).add(new Edge(p, p3));
			graph.get(p3).add(new Edge(p3, p));
		}
		helpers.put(e2, p);
	}
	
	private void handleRegular(Vertex p, TreeSet<Line2D> SL, HashMap<Line2D, Vertex> helpers){
		if(p.getNeighbor(0).getY() > p.getY()){
			int idx = vertices.indexOf(p.getNeighbor(0));
			Line2D e = lines.get(idx);
			
			Vertex p2 = helpers.get(e);
			if(p2.getType() == Type.MERGE){
				lines.add(new Line2D.Double(p, p2));
				graph.get(p).add(new Edge(p, p2));
				graph.get(p2).add(new Edge(p2, p));
			}
			SL.remove(e);
			Line2D e1 = lines.get(vertices.indexOf(p));
			SL.add(e1);
			helpers.put(e1, p);
		}else{
			Line2D e = SL.lower(new Line2D.Double(p, p));
			Vertex p2 = helpers.get(e);
			
			if(p2.getType() == Type.MERGE){
				lines.add(new Line2D.Double(p, p2));
				graph.get(p).add(new Edge(p, p2));
				graph.get(p2).add(new Edge(p2, p));
			}
			helpers.put(e, p);
		}
	}
	
}