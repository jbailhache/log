// package com.jacquesb.wifiinetserv;

/*
import android.app.*;
import android.os.*;
import android.view.*;
import android.widget.*;
*/

/*
import java.io.*;
import java.net.*;

import java.util.*;
import java.lang.*;
import java.nio.*;
import java.math.*;
import java.text.*;
*/

import java.io.*;
import java.net.*;
import java.util.*;
import java.text.*;

/*
import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;    
import android.content.Intent;     
import android.content.IntentFilter;    
import android.net.wifi.ScanResult;    
import android.net.wifi.WifiConfiguration;   
import android.net.wifi.WifiManager;    
import android.os.Bundle;    
import android.util.Log;   
import android.view.View;    
import android.view.View.OnClickListener;    
import android.widget.*;
import android.widget.LinearLayout.*;
import android.net.wifi.*;
import android.net.*;

import android.os.*;
import android.os.PowerManager.*;
*/

// public class WifiInternetServer extends Activity
public class InetMessagesServer
{
   public static class MainActivity 
   {
	public boolean verbose = false;
	// static WifiInternetServer ma;
	/*
	EditText text1;
	TextView textStatus; 
	Button buttonServer, buttonClient, buttonSet, buttonSend;
	EditText editMyId, editParam, editMessages, editTo, editData; 
	Spinner spinnerParam;
	*/

	String me = "myuserid";

	public boolean connected;
	public boolean clientRunning;
	
	// WakeLock wl;
	
	int maxMessageList = 3000;
	int maxMessageSent = 1000;
	
	long selectedParam = 0;
	
	String ssidPrefix = "WFM";
	int port = 1234;
	// String userId = "me";
	String groups = "";
	
	int scanPeriod = 60;
	
	ServerSocket server;
	
	/*
	public void trace(String s)
	{
		Toast.makeText(this, "WifiClient: " + s, 
			Toast.LENGTH_LONG).show(); 
		text1.append("\nmain: " + s);
		Log.d("WifiClient", s);
	}
	*/

	public void trace (final String s)
	{
		System.out.println("main: " + s);
		/*
		ma.runOnUiThread(new Runnable()
			{
				// @Override
				public void run()
				{
					// text1.append("\nmain: "+s);
					// text1.setSelection(text1.length(), text1.length());
					System.out.println("main: " + s);
				}
			});
		*/
	}
	
	/*
	public String encode(String s)
	{
		return s
		.replaceAll("\\","\\\\")
		.replaceAll("\\n","\\\\n")
		.replaceAll("|","\\p");
		
	}
	
	public String decode(String s)
	{
		return s
		.replaceAll("\\n","\n")
		.replaceAll("\\p","|")
		.replaceAll("\\\\","\\");
	}
	*/


	public String encode(String s)
	{
		return s
		.replaceAll("\\\\","\\\\\\\\")
		.replaceAll("\\n","\\\\n")
		.replaceAll("\\|","\\\\p")
		;
		
	}
	
	public String decode(String s)
	{
		return s
		.replaceAll("\\\\n","\n")
		.replaceAll("\\\\p","\\|")
		.replaceAll("\\\\\\\\","\\\\")
		;
	}

	public void saveParams()
	{
		try
		{
			 // PrintWriter out = new PrintWriter(openFileOutput("params.txt", MODE_PRIVATE));
			 PrintWriter out = new PrintWriter(new FileOutputStream("params.txt"));
			 out.println(ssidPrefix);
			 out.println(Integer.toString(port));
			 out.println(me);
			 out.println(groups);
			 out.println(Integer.toString(maxMessageList));
			 out.println(Integer.toString(maxMessageSent));
			 out.println(Integer.toString(scanPeriod));
			 out.close();	
		}
		catch (Exception e)
		{
			trace(e.toString());
		}
	}
	
	public void readParams()
	{
		try
		{
			// BufferedReader in = new BufferedReader(new InputStreamReader(openFileInput("params.txt")));
			BufferedReader in = new BufferedReader(new InputStreamReader(new FileInputStream("params.txt")));
			ssidPrefix = in.readLine();
			port = Integer.parseInt(in.readLine());
			me = in.readLine();
			groups = in.readLine();
			maxMessageList = Integer.parseInt(in.readLine());
			maxMessageSent = Integer.parseInt(in.readLine());
			scanPeriod = Integer.parseInt(in.readLine());
			in.close();
		}
		catch (Exception e)
		{
			trace(e.toString());
		}
	}
	
	public void saveMessages()
	{
		try
		{
			// PrintWriter out = new PrintWriter(openFileOutput("tosend.txt", MODE_PRIVATE)); 
			PrintWriter out = new PrintWriter(new FileOutputStream("tosend.txt")); 
			MessageList l = toSend;
			while (!(l.isEmpty()))
			{
				out.println(l.first.intForm());
				l = l.next;
			}
			out.close();
			// out = new PrintWriter(openFileOutput("received.txt", MODE_PRIVATE));
			out = new PrintWriter(new FileOutputStream("received.txt"));
			l = received;
			while (!(l.isEmpty()))
			{
				out.println(l.first.intForm());
				l = l.next;
			}
			out.close();
		}
		catch (Exception e)
		{
			trace(e.toString());
		}
	}
	
	public void readMessages()
	{
		BufferedReader in = null;
		String line;
		MessageList p, n;
		Message m;
		try
		{
			// in = new BufferedReader(new InputStreamReader(openFileInput("tosend.txt")));
			in = new BufferedReader(new InputStreamReader(new FileInputStream("tosend.txt")));
			/*
			while (true)
			{
				line = in.readLine();
				trace("toSend: line read: " + line);
				toSend = toSend.add(new Message(line));
			}
			*/
			toSend = new MessageList();
			p = toSend;
			while (true)
			{
				line = in.readLine();
				m = new Message(line);
				n = new MessageList();
				p.first = m;
				p.next = n;
				p = n;
			}
		}
		catch (Exception e1)
		{
			trace(e1.toString());
			try
			{
				in.close();
				// in = new BufferedReader(new InputStreamReader(openFileInput("received.txt")));
				in = new BufferedReader(new InputStreamReader(new FileInputStream("received.txt")));
				/*
				while (true)
				{
					line = in.readLine();
					trace("received: line read: " + line);
					received = received.add(new Message(line));
				}
				*/
				received = new MessageList();
				p = received;
				while (true)
				{
					line = in.readLine();
					m = new Message(line);
					n = new MessageList();
					p.first = m;
					p.next = n;
					p = n;
				}
			}
			catch (Exception e2)
			{
				trace(e2.toString());
				try
				{
					in.close();
				}
				catch (Exception e3)
				{
					trace(e3.toString());
				}
			}
		}
	}
	
	/*
	public void wakeLock()
	{
		PowerManager mgr = (PowerManager)this.getSystemService(Context.POWER_SERVICE); 
		wl = mgr.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "WifiServerWakeLock");
		wl.acquire();
	}
	
	public void wakeUnlock()
	{
		wl.release();
	}
	*/

	public String formatTime(long ms)
	{
		DateFormat fmt = new SimpleDateFormat("dd/MM/yyyy HH:mm");
		Calendar cal = Calendar.getInstance();
		cal.setTimeInMillis(ms);
		return fmt.format(cal.getTime());
	}
	
	public long nowSeconds()
	{
		return System.currentTimeMillis() / 1000;
	}

	public class Message
	{
		public String from, to, via, data;
		public long time;
		public boolean isNew;
		
		public Message()
		{
			
		}

		public Message(String line)
		{
			String fields[] = line.split("\\|");
			// Message m = new Message();
			from = decode(fields[0]);
			to = decode(fields[1]);
			// trace("to: " + to);
			via = decode(fields[2]) + ";" + me;
			// trace("via: " + via);
			data = decode(fields[3]);
			// trace("get time "+fields[4]);
			time = Long.parseLong(fields[4]);
			// trace("time: "+time);
			isNew = true;
		}

		public String intForm()
		{
			return encode(from) + "|" +
				encode(to) + "|" +
				encode(via) + "|" +
				encode(data) + "|" +
				new Long(time).toString();
		}
		
		public String extForm()
		{
			return formatTime(time) + " " + from + " to " + to + " : " + data + "\n";
		}
		
		public boolean same(Message m)
		{
			// trace("begin same");
			boolean r;
			r = 
				from.equals(m.from) &&
				to.equals(m.to) &&
				time == m.time;
			// trace("same=" + r + " for " + this + " and " + m);
			return r;
		}
		
		public boolean isForMe()
		{
			int i, j;
			String toArray[] = to.split(",");
			String groupsArray[] = groups.split(",");
			for (i=0; i<toArray.length; i++)
			{
				if (toArray[i].equals(me))
					return true;
				for (j=0; j<groupsArray.length; j++)
				{
					if (toArray[i].equals(groupsArray[j]))
						return true;
				}	
			}
			return false;
		}
	}

	public class MessageList
	{
		public Message first;
		public MessageList next;

		public MessageList()
		{
			first = null;
			next = null;
		}

		public MessageList(Message f, MessageList n) 
		{
			first = f;
			next = n;
		}

		public boolean isEmpty()
		{
			return first == null;
		}

		public MessageList add (Message m)
		{
			trace("add");
			boolean found = false;
			// trace("l");
			MessageList l = this;
			// trace("loop");
			while (!(l.isEmpty()) && !found)
			{
				// trace("in loop");
				if (l.first.same(m))
				{
					// trace("same");
					found = true;
					m.isNew = false;
				}
				else
				{
					// trace("not same");
					l = l.next;
				}
			}
			// trace("found="+found);
			if (found)
			{
				// trace("found, not added: " + m);
				return this;
			}
			// trace("not found, added: " + m);
			truncate(maxMessageList-1);
			// l = new MessageList(m, truncate(3));
			l = new MessageList(m, this);
			// l.truncate(3);
			return l;
		}

		public void truncate(int n)
		{
			if (isEmpty())
				return;
			if (n == 0)
			{
				first = null;
				next = null;
				return;
			}
			next.truncate(n-1);
		}
		
		public MessageList truncate2(int n)
		{
			if (isEmpty())
				return this;
			if (n == 0)
				return new MessageList();
			// return next.truncate(n-1).add(first);
			return new MessageList(first, next.truncate2(n-1));
		}
		
		public void send (PrintWriter out)
		{
            String line;
			MessageList l = this;
			try
			{

				int i = 0;
				trace("before send loop l="+l); 
				// while (l != null)
				while (!l.isEmpty())
				{
					if (i >= maxMessageSent) break;
					i++;
					line = l.first.intForm();
					/*
					 l.first.from + "|" +
					 l.first.to + "|" +
					 l.first.via + "|" +
					 l.first.data + "|" +
					 new Long(l.first.time).toString();
					 */
					trace("send: " + line);
					out.println(line);
					l = l.next;
				}
				trace("after send loop");
			}
			catch (Exception e)
			{
				trace(e.toString());
			}
		}
	}
	
	MessageList toSend, received;
	Message message;

	public void appendMessage(final String s)
	{
		System.out.println("message: " + s);
		/*
		ma.runOnUiThread(new Runnable()
			{
				// @Override
				public void run()
				{
					// editMessages.append(s);
					// editMessages.setSelection(editMessages.length(), editMessages.length());
					System.out.println("message: " + s);
				}
			});
		*/
	}

	public void receiveMessages(BufferedReader in)
	{
		String line;
		try
		{

			while (true)
			{
				trace("read line");
				line = in.readLine();
				trace("received: " + line);
				if (line.equals("END"))
					break;
				/*
				 String fields[] = line.split("\\|");
				 Message m = new Message();
				 m.from = fields[0];
				 m.to = fields[1];
				 trace("to: " + m.to);
				 m.via = fields[2] + ";" + me;
				 trace("via: " + m.via);
				 m.data = fields[3];
				 trace("get time "+fields[4]);
				 m.time = Long.parseLong(fields[4]);
				 m.isNew = true;
				 trace("time: "+m.time);
				 */
				Message m = new Message(line);
				/*
				 received = new MessageList(m, received);
				 toSend = new MessageList(m, toSend); 
				 */
				received = received.add(m);
				// toSend = toSend.add(m);
				// if (m.isNew && m.to.equals(me))
				if (m.isNew && m.isForMe())
				{
					trace("display message");
					// editMessages.append(m.from + ": " + m.data + "\n");
					// appendMessage(formatTime(m.time) + " " + m.from + " to " + m.to + " : " + m.data + "\n");
					appendMessage(m.extForm());
				}
			}

			saveMessages();
		}
		catch (Exception e)
		{
			trace(e.toString());
		}
	}
	

	// public class ServerTask extends AsyncTask <Void, Void, Void>
	public class ServerTask extends Thread
	{

		// EditText text1;
		// WifiInternetServer ma;
		// ServerSocket server;

		public void trace (final String s)
		{
			System.out.println("server: " + s);
			/*
			// Toast.makeText(ma, "Server: " + s, Toast.LENGTH_LONG).show();
			// text1.append("\n" + s);
			ma.runOnUiThread(new Runnable()
				{
					// @Override
					public void run()
					{
						// text1.append("\n(task) "+s);
						// text1.setSelection(text1.length(), text1.length());
						System.out.println(s);
					}
				});
			*/
		}

		public void appendMessage(final String s)
		{
			System.out.println("message: " + s);
			/*
			ma.runOnUiThread(new Runnable()
				{
					// @Override
					public void run()
					{
						// editMessages.append(s);
						// editMessages.setSelection(editMessages.length(), editMessages.length());
						System.out.println("message: " + s);
					}
				});
			*/
		}

		// public ServerTask (WifiInternetServer ma)
		public ServerTask()
		{
			super();
			// this.ma = ma1;

		}

		// @Override
		// protected Void doInBackground (Void... params)
		public void run()
		{
			// Looper.prepare();

			// text1 = (EditText) ma.findViewById(R.id.text1);
			trace("doInBackground");

			try
			{

				// int port = 1234;
				trace("Server");
				server = null;
				try
				{
					server = new ServerSocket(port);
					// server.setSoTimeout(2000);
				}
				catch (Exception e)
				{
					trace("Cannot initialize server: " + e);
				}

				trace("Loop");

				while (true)
				{

					trace("Waiting");
					Socket client = server.accept();
					trace("Connected");

					BufferedReader in = new BufferedReader(
						new InputStreamReader(client.getInputStream()));

					// PrintStream out = new PrintStream(client.getOutputStream());
					PrintWriter out = new PrintWriter(client.getOutputStream(),true); 
					/*
					 trace("Send");
					 out.println("Hello from server");
					 trace("Read");
					 String message = in.readLine();
					 trace ("Received: " + message);
					 // Toast.makeText(this, "Received: " + message, Toast.LENGTH_LONG).show();
					 */

					MessageList l = toSend;
					int i = 0;
					String line;
					
					while (true)
					{

						line = in.readLine();

						if (line.equals("GETTSD"))
						{
							toSend.send(out);
							out.println("END");					
						}
						else if (line.equals("GETRCD"))
						{
							received.send(out);
							out.println("END"); 
						}
						else if (line.equals("PUT"))
						{
							receiveMessages(in);
						}
						else if (line.equals("QUIT"))
						{
							break;
						}
					}
					
					// saveMessages();

					client.close();
					trace("Disconnected");

				}	


			}
			catch (Exception e)
			{
				// Toast.makeText(this, "Exception: " + e.getMessage(), Toast.LENGTH_LONG).show();
				trace("Exception: " + e); 
			}

			// return null;
		}

		/*
		// @Override
		protected void onPostExecute(Void result)
		{
			trace("onPostExecute");
			super.onPostExecute(result);
		}

		// @Override
		protected void onPreExecute()
		{

		}

		// @Override
		protected void onProgressUpdate(Void... values)
		{

		}
		*/

	}
	
	// public static void main (String args[])
	public MainActivity()
	{
		try
		{
			BufferedReader in = new BufferedReader( new InputStreamReader( System.in ));
			String reponse;

			// ma = this;
			clientRunning = false;

			readParams();

			System.out.print("Port ? ");
			reponse = in.readLine();
			port = Integer.parseInt(reponse);
		
			System.out.print("Messages kept ? ");
			reponse = in.readLine();
			maxMessageList = Integer.parseInt(reponse);
			
			System.out.print("Messages sent ? ");
			reponse = in.readLine();
			maxMessageSent = Integer.parseInt(reponse);

			saveParams();

			toSend = new MessageList();
			received = new MessageList();
			readMessages();
			MessageList l = received;
			String s = "";
			while (!(l.isEmpty()))
			{
				if (l.first.isForMe())
				{
					s = "message: " + l.first.extForm() + s; 
				}
				l = l.next;
			}
			System.out.println(s);
	
			trace("button Start");
			// textStatus.append(" server");
			// wakeLock();
			// me = editMyId.getText().toString();
			ServerTask task = new ServerTask();
			trace("execute task");
			// task.execute();
			task.start();
			trace("done Start");
		}
		catch (Exception e)
		{
			trace(e.toString());
		}


	}	
	
	/*
	// @Override
	public void onStop(Bundle state)
	{
		try
		{
			server.close();
		}
		catch (Exception e)
		{
			trace("Error closing server socket: " + e);
		}
		wakeUnlock();
		super.onStop();
	}
	*/

   }

   static MainActivity ma;
	
   public static void main (String args[])
   {
	ma = new MainActivity();
   }
}

