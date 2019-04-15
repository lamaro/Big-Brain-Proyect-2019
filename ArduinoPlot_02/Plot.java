import processing.core.PApplet;

public class Plot {
	
	PApplet parent;	
	int posX,posY,w,h;
	int colorFill;
	int colorStroke;
	int[] points;
	int size = 2;
	int timeRefresh = 5;
	
	boolean vertical;
	
	public Plot(PApplet p,int pX, int pY, int w_, int h_, boolean ver){
		parent = p;
		vertical = ver;
		
		posX=pX;
		posY=pY;
		w=w_;
		h=h_;
		
		if(vertical)
			points = new int[h];
		else
			points = new int[w];
		
		colorFill=parent.color(0);		
		colorStroke=parent.color(255);
		parent.fill(colorFill);
		parent.stroke(colorStroke);
		parent.rectMode(PApplet.CENTER);		
		parent.rect(posX,posY,w,h);
		
	}

	public void draw(float fY){				
		int x;
		int y;
		if(vertical){
			x = (posY-h/2)+(parent.frameCount%h);		
			y = (posX+w/2)-(int)(fY*w);
			//parent.ellipse(y,x,size,size);
                        if(x>0){
                            parent.stroke(colorFill);
                            parent.line(posY-(h*0.5f),x,posY+(h*0.5f),x);
                            parent.stroke(colorStroke);
                            parent.strokeWeight(2);
                            parent.line(points[x-1],x-1,y,x);
                        }
			points[x] = y;
		}else{
			x = (posX-w/2)+(parent.frameCount%w);			
			y = (posY+h/2)-(int)(fY*h);
			//parent.ellipse(x,y,size,size);
                        if(x>0){
                            parent.stroke(colorFill);
                            parent.line(x,posY-(h*0.5f),x,posY+(h*0.5f));
                            parent.stroke(colorStroke);
                            parent.strokeWeight(2);
                            parent.line(x-1,points[x-1],x,y);
                        }
			points[x] = y;
		}
            parent.noFill();
            parent.stroke(colorStroke);  
            parent.rect(posX,posY,w,h);				
	}
}
