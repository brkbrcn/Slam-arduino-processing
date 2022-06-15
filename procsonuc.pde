import cc.arduino.*;
import org.firmata.*;

import processing.serial.*;


Serial mySerial;
String myString =Serial.list()[0];

int[] diziseri = new int [9];//aldıklarımızı tuttugumuz dizi(serialinarray)
int adetbyte = 0;//ne kadar byte aldığımızı sayacak(serialcount)
boolean ilkbag = false;
//int nl = 10;
//float myVal;
int aci;
int uzk;
float yon_durum;
int say=0;
int orjinsay=0;
int optik_uzk;
int optik_haf;
int apsX;
long apsY;
int apsY_ek;
int sag_yon;
int sol_yon;
int carp;

float apsisx=250;
float apsisy=250;
float apsisxhaf;
float apsisyhaf;

void setup(){
  background(0);
  frameRate(300); 
  size(500,500);
 //ilk başlangıç orjini 
 
  //String myPort = Serial.list()[1];
  String myString =Serial.list()[0];
  mySerial=new Serial(this, "COM5",9600);
   //mySerial= new Serial(this, Serial.list()[0], 9600);
}

void draw() {
         
        
  
           tara();
     /*
    if(uzk<101 || uzk>0){
       rectMode(CENTER);
       rect(width/2,height-(uzk/2),100,uzk);
    } */
       
        
     delay(250);
}
void serialEvent(Serial mySerial){//haberleşmeyi sağlayan fonksiyon
                   
                    int gelenbyte = mySerial.read();//inbyte
                            if( ilkbag==false){
                                  if(gelenbyte== 'A'){
                                                        mySerial.clear();
                                                        ilkbag=true;
                                                        mySerial.write('A');
                                              }  }
                                else {//bağlantı varsa verileri al
                                        diziseri[adetbyte] = gelenbyte;
                                        adetbyte++;
                                            if(adetbyte>7){
                                    
                                                      aci=diziseri[3];
                                                      
                                                      sag_yon=diziseri[4];
                                                      
                                                      uzk=diziseri[2];
                                  
                                                      optik_uzk=diziseri[5];
                                                      
                                                      //apsX=(diziseri[6])+100;
                                                      
                                                 //     apsY=(diziseri[0])+300;
                                                      
                                                      sol_yon=diziseri[7];
                                    
                                                      carp=diziseri[1];
                                                      
                                                      
                                                      println("uzk ",uzk + "\t" + "optik_uzk ",optik_uzk+" \t"+ "aci ",aci + " \t"  ,"sag_yon   ",sag_yon+" \t","sol yon=", sol_yon+"   X = ",apsisx +"\t" , "Y = ",apsisy +" \t","carpan =",carp);
                                                      mySerial.write('A');
                                                     
                                                      
                                                                  yon_durum=(sag_yon*90)-(sol_yon*90);
                                                                  
                                                                   
                                                                   if(yon_durum==180){yon_durum=yon_durum*(-1);}
                                                                  if(yon_durum==270){yon_durum=-90;}
                                                                  if(yon_durum==-270){yon_durum=90;}
                                                                  if(yon_durum>=360){yon_durum=yon_durum % 360;}
                                                                  
                                                                   println("yon durumu ",yon_durum);
                                                                   
                                                                   apsisxhaf=apsisx;
                                                                   apsisyhaf=apsisy;
                                                                  
                                                     if(yon_durum==0 && carp==1){apsisy=apsisy-(1.51181102364*15);}//EĞER İLERİ GİDİYOR İSE...
                                                      // println("sıfırladın ve ",gelenbyte,diziseri[1]);
                                                       
                                                     if(yon_durum==90 && carp==1){apsisx=apsisx+(1.51181102364*15);}//sağ yön için..
                                                       
                                                     if(yon_durum==-180 && carp==1){apsisy=apsisy+(1.51181102364*15);}//geri yön için
                                                       
                                                     if(yon_durum==-90 && carp==1){apsisx=apsisx-(1.51181102364*15);}//sol yön için (270 derece)
                                                     
                                                     
                                                   
                                                     if(yon_durum==0 && carp==2){apsisy=apsisy+(1.51181102364*15);}//EĞER İLERİ GİDİYOR İSE...
                                                      // println("sıfırladın ve ",gelenbyte,diziseri[1]);
                                                       
                                                     if(yon_durum==90 && carp==2){apsisx=apsisx-(1.51181102364*15);}//sağ yön için..
                                                       
                                                     if(yon_durum==-180 && carp==2){apsisy=apsisy-(1.51181102364*15);}//geri yön için
                                                       
                                                     if(yon_durum==-90 && carp==2){apsisx=apsisx+(1.51181102364*15);}//sol yön için (270 derece)
                                                      
                                                      adetbyte=0;//dizi sıfırlama
                                                       
                                  }}
                                  }
  
  void tara(){
   
if(say==0){say++;}//ilk andaki parazitleri önlemek için
else{
                            
                              pushMatrix();//sadece etkilenecek fonksiyon alanlarını belirler popmatrix ile birlikte kullanılır gir/çık
                               //translate( apsisx,apsisy);
                               
                               translate( apsisx,apsisy);
                                
                                /*araç döndüğünde bütün koordinat sistemi dönmelidir ve bu dönmeyi push-pop matrix arasında yapıyoruz
                                bu durumda programın kendi kaynak kodundaki dönme fonksiyonu ile bizim yazmış olduğumuz dönme foksiyonu
                                toplanıyor bu durumu düzeltmek için dönüşlerde ayrı ayrı eksen düzeltmesi yapılmıştır*/
                                
                                 if(yon_durum==0){ 
                                rotate(yon_durum);}
                                
                                if(yon_durum==90){
                                  rotate(yon_durum-45);}//90 da doğru
                                  
                                else if(yon_durum==-180){
                                rotate(yon_durum+45);}
                                
                                 else if(yon_durum==-90){
                                rotate(yon_durum);}
                              
                              scale(1);//genişletme(x,y) de olabilir fakat kordinat sistemini bozar
                             
                           /*  stroke(150,50,255);
                            strokeWeight(6);
                              point(0,0); //merkezi belirten nokta*/
                              
                              pushMatrix();//merkezde ki trasnparan geçiş bölümü
                               strokeWeight(1);//merkeze çizilen dikdörtgen kalınlığı
                              stroke(0);//merkeze çizilen dikdörtgen rengi
                              rectMode(CENTER);  // //merkeze çizilen dikdörtgen modu (merkez)
                              fill(0,50);////merkeze çizilen dikdörtgen transparanlık(renk,transparanlık)
                              rect(0,0,54,54);//merkeze çizilen dikdörtgen boyutu
                              fill(255);//merkeze çizilen dikdörtgen dışında kalan nesneleri doldurmak için
                              stroke(255,0,255);//merkeze çizilen dikdörtgen dışındaki nesnelerin rengi
                            strokeWeight(6);////merkeze çizilen noktanın büyüklüğü
                              point(0,0); //merkezi belirten nokta
                              popMatrix();
                              
                              stroke(255); //tarama rengi beyaz olsun
                              
                             if(optik_uzk<60&&optik_uzk>0){//uzaklık 46 cm den küçük ise noktala
                              
                              
                             /* strokeWeight(2);//nokta büyüklüğünü
                              stroke(255,50,50);//nokta rengi (ultrasonik)
                               point((uzk*1.51181102364 )*cos(radians((aci))),(uzk*1.51181102364 )*sin(radians((aci+180))));    // ultrasonik çizimi iptal*/
                               if(uzk>4){//ultrasonik sensör 4 cm den düşük ise çizim yapma
                               strokeWeight(2);//nokta büyüklüğünü
                              //nokta rengi
                               point((optik_uzk*1.51181102364 )*cos(radians((aci))),(optik_uzk*1.51181102364 )*sin(radians((aci+180))));
                               optik_haf=optik_uzk;
                               //line((optik_haf*1.51181102364 )*cos(radians((aci-2))),(optik_haf*1.51181102364 )*sin(radians((aci+178))),(optik_uzk*1.51181102364 )*cos(radians((aci))),(optik_uzk*1.51181102364 )*sin(radians((aci+180))));
                               
                               } }
                              popMatrix();
                             
                               // point(0,0);
                               // aci=aci+0.3;
                              delay(10);
                            }  }
                            
   
