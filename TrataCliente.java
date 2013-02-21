import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;  
import java.io.InputStreamReader;
import java.net.Socket;
import java.util.Scanner;  


public class TrataCliente implements Runnable {  
  
    private InputStream cliente;  
    private Servidor servidor;
    private String nomeCliente;
    private String msg="";
    private Scanner s;
    private Socket client;
    private String tempoTotal = new String();
  private TrataTempo start = new TrataTempo();  
	private TrataTempo end = new TrataTempo();
    
    public TrataCliente(InputStream cliente, Servidor servidor, String nomeCliente, Socket client) {  
        this.cliente = cliente;  
        this.servidor = servidor;
        this.nomeCliente = nomeCliente;
        this.client = client;
        start.start();
    }  
     
    public void run() {

    	s=new Scanner(this.cliente); 
    	BufferedReader clientInput = new BufferedReader(new InputStreamReader(cliente));
    	
		    	while (msg != null && s.hasNext() && client.isConnected())//s.hasNextLine())// && msg!=null) //&& !(msg.trim().equals("")))
		    	{
			    			
			        
			    			try {
					        	msg="";
								msg = clientInput.readLine();
								
								if (msg != "" && msg != "null")
								{
					        		System.out.println("Cliente "+nomeCliente+" disse: "+msg);
					        		tempoTotal = end.Tempo(start);
								    System.out.println("Tempo total gasto: "+tempoTotal);
								}
								    
					
			    			} catch (IOException e) {
			    				e.printStackTrace();
			    				servidor.clientes.remove(nomeCliente);
								System.out.println("Cliente "+nomeCliente+" se desconectou do Servidor...");
								s.close();
			    				
			    			   }
				   	
		    	}
		    	
		    	
		    	servidor.clientes.remove(nomeCliente);
				System.out.println("Cliente "+nomeCliente+" se desconectou do Servidor...");
				s.close();
 	
 
    }
    
    

}
