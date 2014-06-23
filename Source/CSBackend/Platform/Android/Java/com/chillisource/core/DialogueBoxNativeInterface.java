/**
 * DialogueBoxNativeInterface.java
 * Chilli Source
 * 
 * Created by Ian Copland on 06/03/2014
 * Copyright 2014 Tag Games. All rights reserved.
 */

package com.chillisource.core;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.widget.Toast;

/**
 * A native interface for communication with native in reference
 * to system dialogue box functionality.
 */
public class DialogueBoxNativeInterface extends INativeInterface
{	
	/**
	 * Member data
	 */
	public static InterfaceIDType InterfaceID = new InterfaceIDType("DialogueBoxNativeInterface");
	/**
	 * Constructor
	 * 
	 * @author Ian Copland
	 */
	public DialogueBoxNativeInterface()
	{
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return Whether or not this implements the given interface.
	 */
	@Override public boolean IsA(InterfaceIDType inInterfaceType) 
	{
		return (inInterfaceType.Equals(InterfaceID));
	}
	/**
	 * Display a toast notification with the given text
	 * 
	 * @author Ian Copland
	 * 
	 * @param The text to display.
	 */
    public void makeToast(final String in_text)
    {
    	CSApplication.get().scheduleUIThreadTask(new Runnable() 
        {
		    public void run() 
		    {
                Toast.makeText(CSApplication.get().getAppContext(), in_text, Toast.LENGTH_LONG).show();
		    }
		});
    }
    /**
     * Display the systems confirmation dialog.
     * 
     * @author Ian Copland
     * 
     * @param ID that is returned in pressed delegate 
     * @param Title text
     * @param Message text
     * @param Confirm text
     * @param Cancel text
     */
    public void showSystemConfirmDialogue(final int in_dialogID, final String in_title, final String in_message, final String in_confirm, final String in_cancel)		
    {
    	final DialogueBoxNativeInterface dialogueBoxNativeInterface = this;
    	CSApplication.get().scheduleUIThreadTask(new Runnable() 
        {
        	public void run()
        	{
        		new AlertDialog.Builder(CSApplication.get().getActivityContext())
        		.setIcon(android.R.drawable.ic_dialog_alert)
        		.setTitle(in_title)
        		.setMessage(in_message)
        		.setPositiveButton(in_confirm, new DialogInterface.OnClickListener() 
        		{
        			@Override public void onClick(DialogInterface dialog, int which) 
        			{
        				dialogueBoxNativeInterface.onDialogueConfirmPressed(in_dialogID);
        			}
        		})
        		.setNegativeButton(in_cancel,new DialogInterface.OnClickListener() 
        		{
        			@Override public void onClick(DialogInterface dialog, int which) 
        			{
        				dialogueBoxNativeInterface.onDialogueCancelPressed(in_dialogID);
        			}
        		})
        		.show();
        	}
        });
    }
    /**
     * Display the systems dialog 
     * 
     * @author Ian Copland
     * 
     * @param ID that is returned in pressed delegate 
     * @param Title text
     * @param Message text
     * @param Confirm text
     */
    public void showSystemDialogue(final int in_dialogID, final String in_title, final String in_message, final String in_confirm)		
    {
    	final DialogueBoxNativeInterface dialogueBoxNativeInterface = this;
    	CSApplication.get().scheduleUIThreadTask(new Runnable() 
        {
        	public void run()
        	{
        		new AlertDialog.Builder(CSApplication.get().getActivityContext())
        		.setIcon(android.R.drawable.ic_dialog_alert)
        		.setTitle(in_title)
        		.setMessage(in_message)
        		.setPositiveButton(in_confirm, new DialogInterface.OnClickListener() 
        		{
        			@Override public void onClick(DialogInterface dialog, int which) 
        			{
        				dialogueBoxNativeInterface.onDialogueConfirmPressed(in_dialogID);
        			}
        		})
        		.show();
        	}
        });
    }
	/**
	 * Calls back down to native on a dialog confirm button
	 * press.
	 * 
	 * @author Ian Copland 
	 * 
	 * @param The ID of the dialogue box.
	 */
	private native void onDialogueConfirmPressed(int in_id);
	/**
	 * Calls back down to native on a dialogue cancel button
	 * press.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The ID of the dialogue box.
	 */
	private native void onDialogueCancelPressed(int in_id);
}
