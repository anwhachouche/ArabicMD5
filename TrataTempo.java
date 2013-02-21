

public class TrataTempo{

  private long timer;

	public void start(){
		timer = System.currentTimeMillis();
	}
	
	public String Tempo(TrataTempo startTime){
		long t = (System.currentTimeMillis() - startTime.timer);
		int ms = (int)(t%1000);
		int seg = (int)((t/1000)%60);
		int min = (int)(((t/1000)/60)%60);
		int hora = (int)((((t/1000)/60)/60)%24);
		int dia = (int)((((t/1000)/60)/60)/24);
	
		return new String(dia+" Dias, "+hora+" Horas, "+min+" Minutos, "+seg+" Segundos, "+ms+" Milissegundos.");
	}


}
