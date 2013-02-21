import java.io.IOException;
import java.io.DataOutputStream; 
import java.net.ServerSocket;  
import java.net.Socket;  
import java.util.ArrayList;  

import javax.swing.JOptionPane;
  
public class Servidor {  
  
    public static void main(String[] args) throws IOException {  
        // inicia o servidor  
        new Servidor(8888).executa();  //8888 é a porta de entrada escolhida
    }  
    private int porta;   
    public ArrayList<String> clientes; //Cria um ArrayList para controlar a quantidade de clientes, a posição de cada um será usada para calcular o Range
    private String nomeCliente; //Vai guardar o IP de cada cliente
    private String password; //Armazena a senha no formado MD5 informada pelo usuário
    private int numClientes; //Numero de clientes conectados ao servidor
    private static int lowRange=33; //Numero da Tabela ASCII (33 equivale a "!")
    private static int highRange=127;////Numero da Tabela ASCII (33 equivale a "~")
    private int found=0; //Se a senha foi encontrada, found=1 senao found=0
    private String start= "1"; //Se start=1 clientes começam a procurar a senha, senao clientes auardam
    private double lowTemp; //Armazena o valor do LowRange de cada cliente
    private double highTemp; //Armazena o valor do HighRange de cada cliente
    private double Range = (highRange - lowRange);
    
    public Servidor (int port) {     
        this.porta = port;  
        this.clientes = new ArrayList<String>();  
    }  
     
    
    
	public void executa () throws IOException {  
         
        ServerSocket servidor = new ServerSocket(this.porta);  
        System.out.println("Servidor ArabicMD5 Inciado!");
        numClientes= Integer.parseInt(JOptionPane.showInputDialog ("Informe a quantidade de Clientes: "));
        password=JOptionPane.showInputDialog ("Informe a senha criptografada: ");
         
        while (true) {  
            // aceita um cliente  
            Socket cliente = servidor.accept(); //Servidor pronto para aceitar conexões de entrada via Socket
                       
            nomeCliente = cliente.getInetAddress().getHostAddress(); // Captuta o IP do cliente que se conectou
            System.out.println("Nova conexão com o cliente "+cliente.getInetAddress().getHostAddress());
        
            this.clientes.add(nomeCliente);  //Adiciona o cliente na lista
            
           // start = JOptionPane.showInputDialog ("Digite 1 para iniciar! ");
       	           
            lowTemp =  (lowRange + ( (Range / numClientes) * (clientes.lastIndexOf(nomeCliente)) )); //Calcula o LowRange para Cada cliente, de acordo com a posição na lista
            highTemp = (lowRange + ( (Range / numClientes) * (clientes.lastIndexOf(nomeCliente)+1) ));//Clcula o HighRange para cada cliente, de acordo com a posição na lista
            
            //Faz o calculo do Range Máximo e Mínimo de acordo com a quantidade de clientes
            //String low = String.valueOf(lowRange + (((highRange - lowRange) / clientes.size()) * (clientes.lastIndexOf(nomeCliente) )));
            
            String low = String.valueOf((int)(Math.ceil(lowTemp))); //Arredonda para cima o valor do LowRange
            String high = String.valueOf((int)(Math.ceil(highTemp))); //Arredonda para cima o valor do HighRange
            
            String encontrou = String.valueOf(found);
           // String inicia = String.valueOf(start);
            
            DataOutputStream clientOutput = new DataOutputStream(cliente.getOutputStream());
            clientOutput.writeBytes (password+low+"."+high+","+encontrou+start); //Envia um conjunto de informações
            		
            System.out.println("Low Range do cliente: "+low); //A cada cliente que se conectar, exibe no console o lowRange que ele pegou
            System.out.println("High Range do cliente: "+high); ////A cada cliente que se conectar, exibe no console o HighRange que ele pegou
            
            TrataCliente tc = new TrataCliente(cliente.getInputStream(), this, nomeCliente, cliente);  //Chama a Rotina TrataCliente para cada conexão
            new Thread(tc).start();
            
            System.out.println("Clientes Conectados: "+clientes); //Exibe no console a lista de clientes conectados
            
        }    
	}
} 
