  #include <SPI.h>
  #include <Ethernet.h>
  #include <MD5.h>
  
  
  byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };   // Informar um MAC Address para o Arduino
  IPAddress serverIP(192,168,1,178);                      // IP do Servidor Java
  IPAddress ip(192,168,1,177);                            //IP do Client
  int serverPort =8888;                                   //Porta do servidor Java
  
  String msg;                                     //Utilizada para enviar mensagens ao servidor Java
  String senha;                                   //Armazena a senha enviada pelo servidor
  char d;                                         //Recebe os bytes da senha MD5 enviada pelo servidor Java
  char c;                                         //Armazena cada Byte recebido da mensagem enviada pelo servidor
  unsigned char* hash;                            //Utilizada na função para gerar MD5
  char* md5str;                                   //Utilizada na função para gerar MD5
  int cont=0;                                     //Contador utilizado no laço principal onde recebe os valores da senha
  int contLow=0;                                  //Contador utilizado no laço que recebe os valores para highRange
  int contHigh=0;                                 //Contador utilizado no laço que recebe os valores para lowRange
  char low[4];                                    //Armazena valores recebidos referentes ao lowRange
  char high[4];                                   //Armazena valores recebidos referentes ao highRange
  int minimo;                                     //Armazena os valores de low em formato int
  int maximo;                                     //Armazena os valores de high em formato int
  char encontrou='0';                             //Armazena o valor recebido referente ao "encontrou senha" 1->encontrou, 0->nao encontrou 
  char start='0';                                 //Armazena o valor recebido referente ao "iniciar calculos" 1->encontrou, 0->nao encontrou
  int led1 = 2;
  int led2 = 3;
  int led3 = 4;
  int led4 = 5;                          //led encontrou senha
  int l, m, n;                           //Declaração das variáveis utilizadas no for para decriptação da senha
  String strasc;
  
  EthernetClient client;             // Inicializa a biblioteca do client Ethernet com o endereço IP e porta do Servidor
  
  void setup() {
        
        Serial.begin(9600);         // Incializa o Debug Serial
        
          pinMode(led1, OUTPUT);
          pinMode(led2, OUTPUT);
          pinMode(led3, OUTPUT);
          pinMode(led4, OUTPUT);
        
        //Atenção!
        //O código abaixo foi comentado, e deve ser utilizado caso NÃO formos utilizar ip fixo para o client, e formos utilizar conexão DHCP
        
        // Inicializa a comunicação Ethernet
        //if(Ethernet.begin(mac) == 0) {
        // Serial.println("Falha ao configurar Ethernet usando DHCP");
        // Serial.println("Utilizando configurações de rede pre-definidas");
         Ethernet.begin(mac, ip);
        //}
          
        delay(1000); // Aguarda um segundo para o Ethernet Shield inicializar
        Serial.println("Conectando ao servidor...");
  }
  
  void piscaLed ()
  {

      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
      delay (100);
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      delay (100);
    
  }
  
  
  void loop()
  {
   
    if (client.connect(serverIP, serverPort))
    {        
        Serial.println("Conectado!");//Envia mensagem via serial
        piscaLed();
           
          while (true) //se conseguir conectar entra no loop infinito
          {           
                while (start != '1') //Fica em loop até o servidor autorizar o cálculo
               {  
                    while (client.available())
                    {                          
                          while (cont<32)
                          {      
                                 delay (150);                           
                                 c= client.read();
                                 senha+=c;
                                 cont+=1;                                
                          }   
                          
                          while (cont>=32 && c!='.')
                          {     
                                delay (150);
                                c= client.read();
                                
                                   if (c!='.'){
                                        low[contLow]=c;
                                        contLow+=1;
                                        cont+=1;
                                   }
                          }
                          while (cont>=34 && c!=',')
                          {
                                delay (150);
                                c= client.read();
                                                 
                                      if (c!=','){
                                        high[contHigh]=c;
                                        contHigh+=1;
                                        cont+=1;
                                   }                      
                          }
                                 
                        delay (150);
                        c = client.read();
                        encontrou =c;
                        delay (150);
                        c = client.read();
                        start = c;
                        maximo = atoi(high);
                        minimo = atoi(low);
                                          
                    }
                    
                }
                    
                    if (senha.length() == 32) //&& start=='1' && encontrou=='0') //definir o tamanho da senha
                    {
                      //Parte que deve tratar a string recebida
                                     
                          Serial.println("Senha recebida: "+senha); //Informa a senha recebida do Java
                          Serial.println(minimo); //Informa o Low Range recebido
                          Serial.println(maximo); //Informa o High Range recebido
                          
                          digitalWrite(led1, HIGH);
                          delay (50);
                          digitalWrite(led2, HIGH);
                          delay (50);
                          digitalWrite(led3, HIGH);
                          delay (2000);
                          piscaLed();
                          piscaLed();
                          piscaLed();
                          

                              do{ //Vai gerando senhas de acordo com o Range informado pelo Servidor Java 
                              

  			            for (l=(minimo-1); l<(maximo); l++) //senhas de 3 digitos                                          
                                            {                                              			
                                                                     
					              for (m=32; m<127; m++) //senhas de 2 digitos
	               				       {

                                                                for (n=33; n<127; n++) //senhas de 1 digito
								 {
								
                                                                    String password = senha; //Parece que se utilziarmos o valor de senha, mesmo sendo uma String dá erro na hora de converter para char
                                                                    unsigned char psw [password.length()]; //Define um char de tamanho password
                                                                    password.toCharArray((char*)psw,password.length()+1);//Esse car recebe o valor do String Password
                
            							
                                                                        if ((m==32)&&(l==(minimo-1)))
                                                                        {
            								   strasc=(String)(char)n;
                                                                           digitalWrite(led1, HIGH);  
                                                                        }
                						        else
                                                                            if ((m>32)&&(l==(minimo-1)))
                                                                             {     
            								       strasc=(String)(char)m;
                                                                               strasc=strasc+(String)(char)n;
                                                                               digitalWrite(led2, HIGH);
                                                                             }
            								     else
                                                                                if (l>(minimo-1))
                                                                                 {
            								             strasc=(String)(char)l;
                                                                                     strasc=strasc+(String)(char)m;
                                                                                     strasc=strasc+(String)(char)n;
                                                                                     digitalWrite(led3, HIGH);
                                                                                     
                                                                                  }
                								        
                      
                                                                          //String strasc = (String)(char)cont; //Parece que se utilziarmos o valor de senha, mesmo sendo uma String dá erro na hora de converter para char
                                                                          unsigned chasc [strasc.length()]; //Define um char de tamanho password
                                                                          strasc.toCharArray((char*)chasc,strasc.length()+1);//Esse car recebe o valor do String Password
                                                                  
                                    
                                                                            //Gera a MD5 hash da String informada
                                                                            hash=MD5::make_hash((char*)chasc); //TEM QUE DEFINIR O RANGE DE SENHAS!!!!!!!!!!!!!!!!!
                                                                            
                                                                            //Gera o digest (hex encoding) do Hash gerado acima
                                                                            md5str = MD5::make_digest(hash, 16); 
                                                                            
                                                                    
                                                                            if (!strcmp((char*)psw, (char*)md5str)) //compara senha gerada com recebida, se for igual entra
                                                                             {    
                                                                                   delay(150);
                                                                                   client.println("Senha encontrada! A senha e: "+strasc);                                                       
            
                                                                                   delay (100);
                                                                                   Serial.println("Senha encontrada! A senha e: "+strasc); 
                                                                                   delay(1000);
                                                                                   
                                                                                   piscaLed();
                                                                                   piscaLed();
                                                                                   piscaLed();
                                                                                   piscaLed();
                                                                                   piscaLed();
                                                                                   piscaLed();
                                                                                   piscaLed();
                                                                                   piscaLed();
                                                                                   piscaLed();
                                                                                   piscaLed();
                                                                                  
                                                                                   //client.stop();
                                                                                   //for (;;); 
                                                                                   digitalWrite(led4, HIGH);
                                                                                   senha = "";
                                                                                   encontrou = '1';
                                                                                   return;
                                                                   
                                                                               }
                                                                               
                                                                               //free (hash);
                                                                               free (md5str);
                               
                          			              }//for n
                        
                                                           }//for m
                          			
                          		            }//for l
                          
                                                             
                                    delay(350);
                                    client.println("Senha nao encontrada... =( ");
                                    delay (300);
                                    Serial.println("Senha nao encontrada... =( ");
                                                                                
                                    senha = "";

                            }while (senha.length() == 32 && encontrou == '0' && senha != "");
                            
                            digitalWrite(led1, LOW);
                            digitalWrite(led2, LOW);
                            digitalWrite(led3, LOW);
                            
             }//end if senha
                               
           
             if (senha.length() >2 && senha.length() < 32)
             {  
                delay(2000); //Tempo para enviar a mensagem que encontrou a senha antes de desligar
                Serial.print("Senha nao reconhecida");
                senha = "";
             }
            
            // Se o servidor desconectar, para o cliente
            if (!client.connected()) 
            {
              Serial.println();//Envia mensagem via serial
              Serial.println("Desconectado!");
              client.stop(); 
              for(;;);  //Não faz mais nada
                
            }
            
        }// end while true
     
            
     }//end if client connect 
     
     else {
              // Senão conseguir conexão com o servidor
              Serial.println("Falha na conexão");
              client.stop(); 
              for(;;);  //Não faz mais nada
            }
            
}//end void loop
