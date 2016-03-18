package PolygonTriangulation;

public class Edge {
	Vertex v1;
	Vertex v2;
	boolean visited = false;
	
	public Edge(Vertex v1, Vertex v2){
		this.v1 = v1;
		this.v2 = v2;
	}
	
	public Vertex v1(){
		return v1;
	}
	
	public Vertex v2(){
		return v2;
	}
	
	public void setV1(Vertex v1){
		this.v1 = v1;
	}
	
	public void setV2(Vertex v2){
		this.v2 = v2;
	}
}
