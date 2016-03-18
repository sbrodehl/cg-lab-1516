package PolygonTriangulation;
import java.awt.geom.*;

public class Vertex extends Point2D.Double{
	private static final long serialVersionUID = -5089399071417560589L;
	enum Type{REGULAR, START, END, SPLIT, MERGE};
	
	Vertex a;
	Vertex b;
	Type t;
	
	public Vertex(double a, double b){
		super(a, b);
	}
	
	public Vertex(Point2D p){
		super(p.getX(), p.getY());
	}
	
	public void setNeighbors(Vertex a, Vertex b){
		this.a = a;
		this.b = b;
	}
	
	public void setNeighbor(int n, Vertex v){
		if(n==0){
			a = v;
		}
		if(n==1){
			b = v;
		}
	}
	
	public Vertex getNeighbor(int n){
		if(n==0){
			return a;
		}
		if(n==1){
			return b;
		}
		return null;
	}
	
	public void setType(Type t){
		this.t = t;
	}
	
	public Type getType(){
		return t;
	}
	
	public void setType(){
		if( a != null && b != null){
			if(startOrSplit()){
				double angle = angleBetween2Lines(a, this, b);

				if(angle < Math.PI){
					t = Type.START;
				}else{
					t = Type.SPLIT;
				}
			}else if(endOrMerge()){
				double angle = angleBetween2Lines(a, this, b);
				
				
				if(angle < Math.PI){
					t = Type.END;
				}else{
					t = Type.MERGE;
				}
			}else{	
				t = Type.REGULAR;
			}
			
		}
	}
	
	private boolean startOrSplit(){
		return isAbove(a) && isAbove(b);
	}
	
	private boolean endOrMerge(){
		return isBelow(a) && isBelow(b);
	}
	
	private boolean isAbove(Vertex v){
		if(this.getY() == v.getY())
			return this.getX() < v.getX();
		return this.getY() > v.getY();
	}
	
	private boolean isBelow(Vertex v){
		if(this.getY() == v.getY())
			return this.getX() > v.getX();
		return this.getY() < v.getY();
	}

	public static double angleBetween2Lines(Point2D n1, Point2D a, Point2D n2) {
	    Point2D v1 = new Point2D.Double(n1.getX()-a.getX(), n1.getY()-a.getY());
		Point2D v2 = new Point2D.Double(n2.getX()-a.getX(), n2.getY()-a.getY());
		
		double norm1 = Math.sqrt(v1.getX()*v1.getX()+v1.getY()*v1.getY());
		double norm2 = Math.sqrt(v2.getX()*v2.getX()+v2.getY()*v2.getY());
		
		double scp = v1.getX()*v2.getX() + v1.getY()*v2.getY();
		
		double alpha = Math.acos(scp/(norm1*norm2));
		
		if(leftTurn(n1, a, n2)){
			return alpha;
		}else{
			return 2*Math.PI-alpha;
		}
	}
	
	public static boolean leftTurn(Point2D a,Point2D b,Point2D c) {
		double ux = b.getX()-a.getX();
		double uy = b.getY()-a.getY();
		double vx = c.getX()-a.getX();
		double vy = c.getY()-a.getY();
		return (ux*vy-uy*vx >= 0 || Math.abs(ux*vy-uy*vx) < Math.pow(10, -10));
	}
}
