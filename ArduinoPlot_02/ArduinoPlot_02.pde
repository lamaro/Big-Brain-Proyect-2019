import processing.serial.*;

Serial myPort;      // The serial port
int whichKey = -1;  // Variable to hold keystoke values
int inByte = -1;    // Incoming serial data
String buffer = "";

int DATA_SEP = (int)(':');
int LINE_SEP = (int)('#');

int cantSensores = 4;
String[] datos;
float[] val;

int select=0;

Plot[] plot;

float[] xoff;
boolean simul = false;      
boolean vertical = false;

PImage header;
int headerH=100;

public void setup() {
  size(displayWidth, displayHeight);

  header = loadImage("header.jpg");

  PFont myFont = createFont(PFont.list()[2], 14);
  textFont(myFont);    

  if (!simul) {
    // List all the available serial ports:
    println(Serial.list());

    // I know that the first port in the serial list on my mac
    // is always my  FTDI adaptor, so I open Serial.list()[0].
    // In Windows, this usually opens COM1.
    // Open whatever port is the one you're using.
    String portName = Serial.list()[1];
    myPort = new Serial(this, portName, 9600);
  }
  val = new float[cantSensores];
  xoff = new float[cantSensores];
  datos = new String[cantSensores];
  
  for (int i=0;i<datos.length;i++)datos[i]="";

  plot = new Plot[cantSensores];
  for (int i=0;i<plot.length;i++)
    if (vertical){      
      plot[i] = new Plot(this, (i * width / cantSensores) + width / 6, headerH + (height-headerH)/ 2, width/3, height-headerH, vertical);

    }
    else
    {
      plot[i] = new Plot(this, width / 2, headerH + (i * (height-headerH) / cantSensores) + (height-headerH) / 6, width, (height-headerH) / cantSensores, vertical);
    }
    
    
}

public void draw() {
  //background(0);

  image(header, 0, 0, width, headerH);


  if (simul)
    for (int i = 0; i < plot.length; i++)
      plot[i].draw(simulate(i));      
  else
    for (int i=0;i<plot.length;i++)plot[i].draw(val[i]);

  /*stroke(0);
   text(datos[0], 10, 50);
   text(datos[1], 10, 100);
   text(datos[2], 10, 150);*/
}

public void serialEvent(Serial myPort) {
  inByte = myPort.read();
  if (inByte != LINE_SEP) {
    buffer += (char) inByte;
  }
  else {
    parseSensor(buffer);
    buffer = "";
  }
}

void parseSensor(String buffer) {
  //println(buffer);
  String[] s = buffer.split(":");
      // [0] name
      // [1] pin
      // [2] current
      // [3] norm
      // [4] state
      //  
  try {
    if (s[0].equals("t") ){       
      val[0] = constrain(Float.parseFloat(s[3])-0.5, 0, 1);
   //   println("val temp: " + val[0]);
//      debugSensor("temp", s);
    }
    if (s[0].equals("m") ){       
      val[1] = constrain(Float.parseFloat(s[3]), 0, 1); //valor real
      //val[1] = noise(frameCount*0.005); //valor simulado
//      debugSensor("mic", s);
      //println("val Mic: " + val[1]);
    }
    if (s[0].equals("c") ){       
      val[2] = constrain(Float.parseFloat(s[3]), 0, 1);
//      debugSensor("cap", s);
    }
    if (s[0].equals("l") ){       
      val[3] = constrain(Float.parseFloat(s[3]), 0, 1);
     //debugSensor("lig", s);
    }
  } 
  catch (Exception e) {
    // TODO: handle exception
    e.printStackTrace();
    println("problema");
  }


  //    
  //    if (s != null && s.length > 1) {      
  //        float f = val[select];
  //      try {
  //        f = Float.parseFloat(s[2]);
  //      } 
  //      catch (Exception e) {
  //        // TODO: handle exception
  //      }
  //
}

void debugSensor(String name, String[] data){
      print(name+ ": raw:" + data[2]);            
      print(" norml: " + data[3]);
      print(" stat: " + data[4]);
      println();     
}

//
//if(true) return;
//  
//  if (inByte != LINE_SEP) {
//    if (buffer.equals("")) {
//      select = Integer.parseInt(""+(char) inByte);
//    }
//    buffer += (char) inByte;
//  } 
//  else if (inByte == LINE_SEP) {
//    datos[select] = buffer;
//    String[] s = buffer.split(":");      
//    if (s != null && s.length > 2) {      
//      float f=val[select];
//      try {
//        f = Float.parseFloat(s[2]);
//      } 
//      catch (Exception e) {
//        // TODO: handle exception
//      }
//      val[select]=f;
//    }
//    buffer="";
//  }
//  
public void keyPressed() {
  // Send the keystroke out:
  if (key=='1') {
    myPort.write("M1:1000:1000#");
  }
  else if (key=='2') {
    myPort.write("M2:1000:1000#");
  }
  else if (key=='3') {
    myPort.write("M3:1000:1000#");
  }
  else if (key=='q') {
    myPort.write("Q1:0.99#");
  }
  else if (key=='w') {
    myPort.write("Q2:0.99#");
  }
  else if (key=='e') {
    myPort.write("Q3:0.99#");
  }  
  whichKey = key;
}

float simulate(int i) {
  xoff[i] += (i + 1) * 0.01f;
  return noise(xoff[i]);
}

