/* 
* 
* 
Δίκτυα Υπολογιστών I 
* 
* Experimental Virtual Lab 
* 
* Java virtual modem communications seed code 
* 
*/
import java.io.*;
import java.lang.*;
import java.nio.file.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.Writer;
import java.io.FileWriter;
import java.util.*;

import ithakimodem.Modem;

public class userApplication{
	public static void main(String[] args) throws IOException{
		
		userApplication user;
		user = new userApplication();
		user.connect();
	}
	
	public void connect() throws IOException{
		//connect with a Modem in Ithaki
		Modem modem;
		modem = new Modem();
		modem.setSpeed(80000);
		modem.setTimeout(2000);
		modem.open("ithaki");
		
		//echo request code for packages		
		String echo="E8059\r";
		byte[] bytes = echo.getBytes();
		modem.write(bytes);
		
		long beginTime = System.currentTimeMillis();
		int k = 0;
		int pack = 0;
		int size = 400;
		String echo_msg="";
		String echo_times="";
		
		//Create the files for package response
		File out_echo = new File("/home/kosmas/workspace/ComputerNetworks_I/echo.txt");
		File out_times = new File("/home/kosmas/workspace/ComputerNetworks_I/times.ods");
		FileOutputStream out_echo_str = new FileOutputStream(out_echo);
		FileOutputStream out_times_str = new FileOutputStream(out_times);
		
				
		while (pack < size && (System.currentTimeMillis()-beginTime)<240000){
			String message="";
			long start = System.currentTimeMillis();
			modem.write(bytes);
			
			for (;;){
				try{
					// PSTART DD-MM-YYYY HH-MM-SS PC PSTOP 
					k = modem.read();
					if (k == -1){
						System.out.println("Exit");
						break;
					}
					message += (char)k;
					System.out.print((char) k);
				} catch (Exception x){
					break;
				}
				if (message.indexOf("PSTOP")>-1){
					long end = System.currentTimeMillis();
					long response = end - start;
					echo_msg += message + "\r";
					echo_times += String.valueOf(response) + "\r";
				}
			}
			pack++;
		}
		
		try{
			out_echo_str.write(echo_msg.getBytes());
			out_times_str.write(echo_times.getBytes());
			out_echo_str.close();
			out_times_str.close();
		} catch (Exception x){
			System.out.println(x);
		}
		 
		
		//IMAGE WITHOUT ERRORS		
		String imagestr = "M6096\r";
		byte[] imgbytes = imagestr.getBytes();
		modem.write(imgbytes);
		int  i,j;
		File image = new File("/home/kosmas/workspace/ComputerNetworks_I/E1.jpg");
		try {
			FileOutputStream stream = new FileOutputStream(image);
			int flag = 0;
			i = modem.read(); 
			
			for (;;){
				j = i;
				i = modem.read(); // i = j - 1
				if (i == -1){
					break;
				}
				if (j==255 && i==216){ // beginning of the image file
					flag = 1;
				}
				if (flag == 1){
					try{
						stream.write(j);
						System.out.print((char) j);
					}
					catch(IOException e){
						System.out.println("not written data");
					}
				}
				if (j==255 && i==217){ // end of image file
					break;
				}
			}}
			catch(FileNotFoundException e){
				
			}
		
		//IMAGE WITH ERRORS		
				String imagestr_er = "G3268\r";
				byte[] imgbytes_er = imagestr_er.getBytes();
				modem.write(imgbytes_er);
				File image_er = new File("/home/kosmas/workspace/ComputerNetworks_I/E2.jpg");
				try {
					FileOutputStream stream = new FileOutputStream(image_er);
					int flag = 0;
					i = modem.read(); 
					
					for (;;){
						j = i;
						i = modem.read(); // i = j - 1
						if (i == -1){
							break;
						}
						if (j==255 && i==216){ // beginning of the image file
							flag = 1;
						}
						if (flag == 1){
							try{
								stream.write(j);
								System.out.print((char) j);
							}
							catch(IOException e){
								System.out.println("not written data");
							}
						}
						if (j==255 && i==217){ // end of image file
							break;
						}
						}
					}
					catch(FileNotFoundException e){
						
					}
				
				
				//GPS request			
				String gps_code = "P2745T=225734403738T=225730403730T=225760403750T=222730403130T=222739403135T=225731403731";				
		
				FileWriter TWriter = new FileWriter("T.txt");
				//create gps img
				String gpsWithT;
				StringBuilder gpsT = new StringBuilder();
				gpsT.append(gps_code);
				gpsWithT = gpsT.substring(0);
				//System.out.println(gpsWithT);
				gpsWithT = gpsWithT+"\r";
				try{
					TWriter.append(gpsT);
					TWriter.close();
				}catch(Exception x){
					System.out.println(x);
				}
				
				//satellite img
				modem.write(gpsWithT.getBytes());
				List <Byte> GPSWithTList = new ArrayList <Byte>();
				do{
					k=modem.read();
					GPSWithTList.add((byte)k);
				}while( k != -1);
				
				int size_list = GPSWithTList.size();
				byte[] gpsIMG = new byte[size_list];
				for (j=0; j<size_list;j++){
					gpsIMG[j] = GPSWithTList.get(j);
				}
				try{
					Files.write(Paths.get("M1.jpg"), gpsIMG);
				}catch(Exception x){
					System.out.println(x);
				}
				/*
				String gps_code = "P2745";
				String gps_codeParam = gps_code + "R=1004099" + "\r";
				FileWriter gpsWriter = null;
				FileWriter TWriter = null;
				//make a new file for every gps sign(M1.txt) and param(T.txt)
				try{
					gpsWriter = new FileWriter("M1.txt");
					TWriter = new FileWriter("T.txt");
				}catch(IOException e){
					System.out.println(e);
				}
				//find gps variables
				//scanner path
				Path Source= Paths.get("M1.txt");
				int cc, zz = 0;
				String aabb, ddee, east, north, gpsWithT;
				
				//temp to create T code 
				String TempString1, tempStringzz;
				int tempInt1, tempInt2=0;
				float tempFloat1, tempFloat2;
				StringBuilder gpsT = new StringBuilder();
				
				//gps req
				gpsT.append(gps_code);
				modem.write(gps_codeParam.getBytes());
				//make list
				List <Byte> gpsList = new ArrayList <Byte>();
				do{
					k = modem.read();
					gpsList.add((byte)k);
				}while (k != -1);
				
				try{
					for (Byte str: gpsList){
						gpsWriter.write(str);
					}
					gpsWriter.close();
				}catch(Exception x){
					System.out.println(x);
				}
				
				Scanner gpsScanner = null;
				try{
					gpsScanner = new Scanner(Source);
				}catch(IOException e){
					System.out.println(e);
				}
				gpsScanner.nextLine();
				// find details blah blah
				while (gpsScanner.hasNextLine()){
					if (gpsScanner.nextLine().startsWith("$GPGGA")){
						Scanner gpsLine = new Scanner(gpsScanner.nextLine());
						gpsLine.useDelimiter(",");
						gpsLine.next();
						gpsLine.next();
						north = gpsLine.next();
						
						//choose DDEE
						ddee = north.substring(0,4);
						tempStringzz = north.substring(5,8);
						tempInt1 = Integer.parseInt(tempStringzz);
						tempFloat1 = tempInt1;
						tempFloat1 = tempFloat1/1000;
						tempFloat1 = tempFloat1*60;
						//choose zz
						zz = Math.round(tempFloat1);
						gpsLine.next();
						east = gpsLine.next();
						
						//choose aabb
						aabb = east.substring(1,5);
						TempString1 = east.substring(6,9);
						tempInt2 = Integer.parseInt(TempString1);
						tempFloat2 = tempInt2;
						tempFloat2 = tempFloat2/1000;
						tempFloat2 = tempFloat2*60;
						
						//choose cc
						cc = Math.round(tempFloat2);
						//4 sec gap
						for (i=0; i<4; i++){
							if (gpsScanner.hasNextLine()){
								gpsScanner.nextLine();
							}
						}
						gpsLine.close();
						//create T
						gpsT.append("T="+aabb+cc+ddee+zz);
					}
				}
				//create gps img
				gpsWithT = gpsT.substring(0,117);
				gpsWithT = gpsWithT+"\r";
				try{
					TWriter.append(gpsT);
					TWriter.close();
					gpsScanner.close();
				}catch(Exception x){
					System.out.println(x);
				}
				
				//satellite img
				modem.write(gpsWithT.getBytes());
				List <Byte> GPSWithTList = new ArrayList <Byte>();
				do{
					k=modem.read();
					GPSWithTList.add((byte)k);
				}while( k != -1);
				
				int size_list = GPSWithTList.size();
				byte[] gpsIMG = new byte[size_list];
				for (j=0; j<size_list;j++){
					gpsIMG[j] = GPSWithTList.get(j);
				}
				try{
					Files.write(Paths.get("M1.jpg"), gpsIMG);
				}catch(Exception x){
					System.out.println(x);
				}
				*/
				
				//ARQ request
				int packets = 5000;
				
				File outputARQ = new File("/home/kosmas/workspace/ComputerNetworks_I/arq_msgs.txt");
				File output_timeARQ = new File("/home/kosmas/workspace/ComputerNetworks_I/arq_times.ods");
				File out_triesARQ = new File("/home/kosmas/workspace/ComputerNetworks_I/arq_packages.ods");
				
				FileOutputStream outFileStreamARQ;
				FileOutputStream outTimeStreamARQ;
				FileOutputStream outTriesStreamARQ;
				
				String ACK = "Q4228\r";
				String NACK = "R3687\r";
				String tx_msg = ACK;
				String rx_msg = "";
				String messages = "";
				String times = "";
				String tries = "";
				long duration, t1, t2;
				int req = 0;
				i = 0;
				beginTime = System.currentTimeMillis();
				boolean correct;
				//correct works as a flag
				while (i<packets && (System.currentTimeMillis()-beginTime)<240000){
					correct = false;
					long startTime = System.currentTimeMillis();
					req = 0;
					t1 = System.currentTimeMillis();
					//if is false, check again until true
					while (!correct){
						try{
							modem.write(tx_msg.getBytes());
						}catch (Exception x){
							System.out.println("Error passing the message");
						}
						req++;
						rx_msg="";
						duration = System.currentTimeMillis();
						//if true write
						while(true){
							try{
								if ((k=modem.read())==-1){
									break;
								}
								rx_msg += (char)k;
								System.out.print((char)k);
							}catch(Exception x){
								break;
							}
							//check if it is the end
							if (rx_msg.indexOf("PSTOP")>-1){
								//check if it is correct
								correct = check(rx_msg);
								if (correct){
									long endTime = System.currentTimeMillis();
									long response = duration - startTime;
									tx_msg = ACK;
									t2 = System.currentTimeMillis();
									times += String.valueOf(t2-t1) + "\r";
									messages += rx_msg + "\n";
									tries += String.valueOf(req) + "\r";
								}
								else{
									tx_msg = NACK;
								}
								break;	
							}
						}
					}
					i++;
				}
				//create file and streams
				try{
					outFileStreamARQ = new FileOutputStream(outputARQ);
					outTimeStreamARQ = new FileOutputStream(output_timeARQ);
					outTriesStreamARQ = new FileOutputStream(out_triesARQ);
					
					outFileStreamARQ.write(messages.getBytes());
					outTimeStreamARQ.write(times.getBytes());
					outTriesStreamARQ.write(tries.getBytes());
					outFileStreamARQ.close();
					outTimeStreamARQ.close();
					outTriesStreamARQ.close();					
				}catch(IOException e){
					System.out.println(e);
				}
				
				modem.close();
								
		/*int size = 500;
		int i = 0;
		long start = System.currentTimeMillis();
		String echo_msg = "";
		String echo_time = "";
		//create files for packages and response times
		File output = new File("echo.txt");
		File outputTime = new File("time.ods");
		FileOutputStream output_str = null;
		FileInputStream output_time_str = null;
		//check the time and the size of the package
		
		*/
	}
	
	public static boolean check(String packet){
		int left = packet.indexOf("<");
		int right = packet.indexOf(">");
		int FCS = Integer.parseInt(packet.substring(right+2, right+5));
		int temp = (int) packet.charAt(left + 1);
		int i;
		
		for (i = left+2; i<right; i++){
			temp = temp^(int) packet.charAt(i);
		}
		
		if (temp==FCS){
			return true;
		}
		return false;
	}
	
}