package server;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Scanner;

public class ServerEcho {

	public static void main(String[] args) throws UnknownHostException, IOException {
		// Connect to server socket
		ServerSocket s;
		Socket x;
		PrintWriter output;
		Scanner input;
		String msg1;
		
		s = new ServerSocket(3000); // configure to desired port later
		x = s.accept();
		System.out.println("Connection established\n");
		input = new Scanner(x.getInputStream());
		output = new PrintWriter(x.getOutputStream());
		
		do {
			msg1 = input.nextLine();
			System.out.println("Messaged recieved: " + msg1);
			System.out.println("Sending message back...");
			output.println(msg1);
			
		} while(!msg1.equals("end"));
		
		x.close();
		s.close();
		input.close();
	}

}