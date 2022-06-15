#include <Servo.h>// servo motor kütüphanesi
#include <SharpIR.h>//lidar sensör kütüphanesi

#define trig 2 //ultrasonik sensör için trig ve echo pinleini belirledik
#define echo 3
#define optik A0//lidar sensör ün sinyal ucu
#define model 1080//lidar sensör modeli bu veri kütüphane ile ilişkilidir.
//motor sürücü pinleri belirlenir.
#define SolMotorileri 9
#define SolMotorGeri 10
#define SagMotorileri 11
#define SagMotorGeri 12
#define pwmsag 5
#define pwmsol 6

//kullanılacak değişkenler belirlenir.
float sure;
int on_uzaklik=0;
int sag_uzaklik=0;
int sol_uzaklik=0;

int optik_uzaklik;
int uzaklik;//deger 1
int aci=0;//deger 2
int gelenbyte;//seri gelen byte için(inByte)

int sol_yon=0;
int sag_yon=0;//ilk durum ileri yöndür - veri gönderilemediği için kaç kere sağ sol yaptığı toplanacaktır.(derece)
int yon_durum=0;

int apsisX=0; //cm (koordinat düzleminde 100,300 noktası 0,0 noktası olarak başlayacak )
int apsisY=0;//cm  seri haberşeşmede değer 256 dan büyük ise değer parça parça gönderilmelidir bu durum processingde de düzenlenebilir aynı durum negatifler için de geçerlidir
int carpan;

SharpIR mySensor = SharpIR(optik, model);//optik sensörü tanımlama

Servo servo; //servo motor tanımlama

void setup() {
    
   
      pinMode(echo, INPUT);
      pinMode(trig, OUTPUT);
        pinMode(SolMotorileri, OUTPUT);
        pinMode(SolMotorGeri, OUTPUT);
        pinMode(SagMotorileri, OUTPUT);
        pinMode(SagMotorGeri, OUTPUT);
  // put your setup code here, to run once:
  Serial.begin(9600);
        servo.attach(8); 
  while(!Serial){;}//seri haberleşme varsa işlem yap!

        iletisim();//iletişim fonksiyonunu başlat
  
}


void loop() {

                  if(Serial.available()>0){ // Serial Porta girdi değerinin olup olmadığını kontrol et(Haberleşme için)
                                            gelenbyte = Serial.read(); }
                                                                    
                                                                    
                                                                    carpan=0;//-,+ yön için "seri haberleşmede - veri gönderilmiyor"
                                                                    yon_durum=(sag_yon*90)-(sol_yon*90);
                                                                    if(yon_durum==360||yon_durum==-360){yon_durum=0;
                                                                    sag_yon=0;
                                                                    sol_yon=0;
                                                                        }
              tarama();//tarama fonksiyonunu başlat
              if(aci>=179){//acı 180 olarak tamamlanmadan harekete geçme
                            hareket();//hareket fonksiyonunu başlat
                            }
     
 //seri haberleşmede dizi verilerin gönderildiği kısım
Serial.write(uzaklik);
Serial.write(aci);
Serial.write(sag_yon);
Serial.write(optik_uzaklik);
Serial.write(apsisX);
Serial.write(sol_yon);//deneme verisi
Serial.write(apsisY);
Serial.write(carpan);


delay(50);

}

void iletisim(){//eğer haberleşme yok ise 'A' bilgisini gönder
  while(Serial.available()<=0){
    Serial.println('A');
    
       delay(300);
  }
}
void tarama(){
                  servo.write(aci);//servoyu açı değerine götür
                  delay(100);

                  digitalWrite(trig,LOW);
                  delayMicroseconds(50);
                  digitalWrite(trig,HIGH);
                  delayMicroseconds(100);
                  digitalWrite(trig,LOW);
                  sure= pulseIn(echo,HIGH);
                  uzaklik=sure/29.1/2;//ultrasonik sensör ölçümü yapıldı
                  optik_uzaklik = mySensor.distance()-0,6; //optik sensör ile ultrasonik sensör arasında bağlantıdan kaynaklı 0,6 cm fark var.
                 
                      if(aci==90){//ön uzakığı hafızaya alma
                                            on_uzaklik=uzaklik;
                                                                }

                      if(aci==2){//sol uzakığı hafızaya alma
                                            sag_uzaklik=uzaklik;
                                         }

                      if(aci==178){//sag uzakığı hafızaya alma
                                          sol_uzaklik=uzaklik;
                                         }


                        aci=aci+2;
                        if(aci>180){aci=0;}     }

void kalkis()/*Dc motor 60-80 pwm arası kalkış yapamadığı ve bobin ağlaması durumu gerçekleştiği için
ilk anda tam güç ile kalkış sağlanır bu çok kısa bir süre için devam eder ve motor kendini toparlayınca normal 
ileri fonksiyonu devreye girer.*/

{
  analogWrite(pwmsag,255);//sag ve sol motor ters bağlı
        analogWrite(pwmsol,255);
digitalWrite(SolMotorileri, LOW);
digitalWrite(SolMotorGeri, HIGH);
digitalWrite(SagMotorileri, LOW);
digitalWrite(SagMotorGeri, HIGH);
}

void ileri()
{
  analogWrite(pwmsag,72);//sag ve sol motor ters bağlı
        analogWrite(pwmsol,75);
digitalWrite(SolMotorileri, LOW);
digitalWrite(SolMotorGeri, HIGH);
digitalWrite(SagMotorileri, LOW);
digitalWrite(SagMotorGeri, HIGH);

carpan=1;//seri haberleşmede negatif sayı gönderilmez


}
void dur()
{ analogWrite(pwmsag,0);
        analogWrite(pwmsol,0);
digitalWrite(SolMotorGeri, LOW);
digitalWrite(SolMotorileri, LOW);
digitalWrite(SagMotorileri, LOW);
digitalWrite(SagMotorGeri, LOW); 
}
void sol()
{
  analogWrite(pwmsag,100);
        analogWrite(pwmsol,100);
digitalWrite(SolMotorileri, LOW);
digitalWrite(SolMotorGeri, HIGH);
digitalWrite(SagMotorGeri, LOW);
digitalWrite(SagMotorileri, HIGH);
sol_yon++;
//carpan=4;
}

/*solfren ve sagfren fonksiyonu motor bir yönde dönerken şase uzunluğundan kaynaklı 
  savurmayı önlemek için ters yönde kısa süre motora gerilim verdiğimiz yani
  frenleme yaptığımız fonksiyondur. */

void solfren()
{
  analogWrite(pwmsag,100);
        analogWrite(pwmsol,100);
digitalWrite(SolMotorileri, LOW);
digitalWrite(SolMotorGeri, HIGH);
digitalWrite(SagMotorGeri, LOW);
digitalWrite(SagMotorileri, HIGH);
}
  
void sag()
{  analogWrite(pwmsag,100);
        analogWrite(pwmsol,100);
digitalWrite(SolMotorGeri, LOW);
digitalWrite(SolMotorileri, HIGH);
digitalWrite(SagMotorileri, LOW);
digitalWrite(SagMotorGeri, HIGH); 
sag_yon++;
//carpan=3;
}
void sagfren()
{  analogWrite(pwmsag,100);
        analogWrite(pwmsol,100);
digitalWrite(SolMotorGeri, LOW);
digitalWrite(SolMotorileri, HIGH);
digitalWrite(SagMotorileri, LOW);
digitalWrite(SagMotorGeri, HIGH); 
}
void geri()
{
  analogWrite(pwmsag,75);
        analogWrite(pwmsol,75);
digitalWrite(SolMotorGeri, LOW);
digitalWrite(SolMotorileri, HIGH);
digitalWrite(SagMotorGeri, LOW);
digitalWrite(SagMotorileri, HIGH);
carpan=2;
}
void fren()
{
  analogWrite(pwmsag,75);
        analogWrite(pwmsol,75);
digitalWrite(SolMotorGeri, LOW);
digitalWrite(SolMotorileri, HIGH);
digitalWrite(SagMotorGeri, LOW);
digitalWrite(SagMotorileri, HIGH);
}

void hareket(){

   if(on_uzaklik>30){//ileri durum
              kalkis();
              delay(40);
              ileri();
              delay(350);
              fren();
              delay(70);
              dur();
                    }
    else {

            if(sag_uzaklik > 25 && sag_uzaklik>sol_uzaklik)//sag durum
                            {
                                            sag();
                                            delay(525);//dönüş durumunu 90 dereceye sabitle 
                                            solfren();
                                            delay(100);
                                            dur();
                                                }
            else if(sol_uzaklik > 25 && sol_uzaklik>sag_uzaklik)//sol durum
                            {
                                            sol();
                                            delay(525);//dönüş durumunu 90 dereceye sabitle 
                                            sagfren();
                                            delay(100);
                                            dur();
                                             }

            else if(on_uzaklik < 30 && sag_uzaklik < 25 && sol_uzaklik < 25 ){ //geri durum
                                            geri();
                                            delay(500);//her yön kapalıyken geri ve sağ hareket yapacak
                                           for(int s = 0 ; s < 2; s++){
                                           if(sag_uzaklik>=sol_uzaklik){
                                            sag();
                                            delay(330);//dönüş durumunu 90 dereceye sabitle 
                                            solfren();
                                            delay(100);}
                                            else{
                                              sol();
                                             delay(330);
                                             sagfren();
                                             delay(100);
                                             }}
                                             dur();
                                            
                                              }  }
            on_uzaklik,sag_uzaklik,sol_uzaklik=0;
                                              }
