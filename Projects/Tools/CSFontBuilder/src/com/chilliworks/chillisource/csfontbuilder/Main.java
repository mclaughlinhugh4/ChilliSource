/**
 * Main.java
 * Chilli Source
 * Created by Ian Copland on 26/06/2014.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Tag Games Limited
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

package com.chilliworks.chillisource.csfontbuilder;

import com.chilliworks.chillisource.texturepackerutils.TexturePacker.PlacementHeuristic;
import com.chilliworks.chillisource.toolutils.Logging;
import com.chilliworks.chillisource.toolutils.StringUtils;
import com.chilliworks.chillisource.toolutils.Logging.LoggingLevel;

/**
 * Handles the entry point into the application. This manages the parsing of the input parameters and
 * passes them on to the rest of the tool.
 * 
 * @author Ian Copland
 */
public final class Main
{
	private static final String k_paramInput = "--input";
	private static final String k_paramInputShort = "-i";
	private static final String k_paramOutput = "--output";
	private static final String k_paramOutputShort = "-o";
	private static final String k_paramFixedWidth = "--fixedwidth";
	private static final String k_paramFixedWidthShort = "-fw";
	private static final String k_paramFixedHeight = "--fixedheight";
	private static final String k_paramFixedHeightShort = "-fh";
	private static final String k_paramValidWidths = "--validwidths";
	private static final String k_paramValidWidthsShort = "-vw";
	private static final String k_paramValidHeights = "--validheights";
	private static final String k_paramValidHeightsShort = "-vh";
	private static final String k_paramMaxWidth = "--maxwidth";
	private static final String k_paramMaxWidthShort = "-mw";
	private static final String k_paramMaxHeight = "--maxheight";
	private static final String k_paramMaxHeightShort = "-mh";
	private static final String k_paramDivisibleBy = "--divisibleby";
	private static final String k_paramDivisibleByShort = "-db";
	private static final String k_paramPlacementHeuristic = "--placementheuristic";
	private static final String k_paramPlacementHeuristicShort = "-ph";
	private static final String k_paramLineHeight = "--lineheight";
	private static final String k_paramLineHeightShort = "-lh";
	private static final String k_paramImageCompression = "--imagecompression";
	private static final String k_paramImageCompressionShort = "-ic";
	private static final String k_paramImageFormat = "--imageformat";
	private static final String k_paramImageFormatShort = "-if";
	private static final String k_paramDitherImage = "--ditherimage";
	private static final String k_paramDitherImageShort = "-di";
	private static final String k_paramDisablePremultipliedAlpha = "--disablepremultipliedalpha";
	private static final String k_paramDisablePremultipliedAlphaShort = "-dpa";
	private static final String k_paramHelp = "--help";
	private static final String k_paramHelpShort = "-h";
	private static final String k_placementHeuristicTopLeft = "topleft";
	private static final String k_placementHeuristicBottomRight = "bottomright";
	
	/**
	 * The entry point into the application
	 * 
	 * @author Ian Copland
	 * 
	 * @param The input arguments.
	 */
	public static void main(String[] in_arguments) throws Exception 
	{
		String[] arguments = Logging.start(in_arguments);
		
		if (arguments.length == 0)
		{
			printHelpText();
			return;
		}
		
		FontBuilderOptions options = new FontBuilderOptions();
		for (int i = 0; i < arguments.length; ++i)
		{
			//input
			if (arguments[i].equalsIgnoreCase(k_paramInput) == true || arguments[i].equalsIgnoreCase(k_paramInputShort) == true)
			{
				if (i+1 < arguments.length)
					options.m_inputDirectoryPath = StringUtils.standardiseDirectoryPath(arguments[i+1]);
				else
					Logging.logFatal("No input directory path provided!");
				i++;
			}
			
			//output
			else if (arguments[i].equalsIgnoreCase(k_paramOutput) == true || arguments[i].equalsIgnoreCase(k_paramOutputShort) == true)
			{
				if (i+1 < arguments.length)
					options.m_outputFilePath = StringUtils.standardiseFilePath(arguments[i+1]);
				else
					Logging.logFatal("No output file path provided!");
				i++;
			}
			
			//fixed width
			else if (arguments[i].equalsIgnoreCase(k_paramFixedWidth) == true || arguments[i].equalsIgnoreCase(k_paramFixedWidthShort) == true)
			{
				if (i+1 < arguments.length)
					options.m_fixedWidth = Integer.parseInt(arguments[i+1]);
				else
					Logging.logFatal("No fixed width provided!");
				i++;
			}
			
			//fixed height
			else if (arguments[i].equalsIgnoreCase(k_paramFixedHeight) == true || arguments[i].equalsIgnoreCase(k_paramFixedHeightShort) == true)
			{
				if (i+1 < arguments.length)
					options.m_fixedHeight = Integer.parseInt(arguments[i+1]);
				else
					Logging.logFatal("No fixed height provided!");
				i++;
			}
			
			//valid widths
			else if (arguments[i].equalsIgnoreCase(k_paramValidWidths) == true || arguments[i].equalsIgnoreCase(k_paramValidWidthsShort) == true)
			{
				if (i+1 < arguments.length)
					options.m_validWidths = parseIntegerArray(arguments[i+1]);
				else
					Logging.logFatal("No valid widths provided!");
				i++;
			}
			
			//valid heights
			else if (arguments[i].equalsIgnoreCase(k_paramValidHeights) == true || arguments[i].equalsIgnoreCase(k_paramValidHeightsShort) == true)
			{
				if (i+1 < arguments.length)
					options.m_validHeights = parseIntegerArray(arguments[i+1]);
				else
					Logging.logFatal("No valid heights provided!");
				i++;
			}
			
			//max width
			else if (arguments[i].equalsIgnoreCase(k_paramMaxWidth) == true || arguments[i].equalsIgnoreCase(k_paramMaxWidthShort) == true)
			{
				if (i+1 < arguments.length)
					options.m_maxWidth = Integer.parseInt(arguments[i+1]);
				else
					Logging.logFatal("No max width provided!");
				i++;
			}
			
			//max height
			else if (arguments[i].equalsIgnoreCase(k_paramMaxHeight) == true || arguments[i].equalsIgnoreCase(k_paramMaxHeightShort) == true)
			{
				if (i+1 < arguments.length)
					options.m_maxHeight = Integer.parseInt(arguments[i+1]);
				else
					Logging.logFatal("No max height provided!");
				i++;
			}
			
			//divisible by
			else if (arguments[i].equalsIgnoreCase(k_paramDivisibleBy) == true || arguments[i].equalsIgnoreCase(k_paramDivisibleByShort) == true)
			{
				if (i+1 < arguments.length)
					options.m_divisibleBy = Integer.parseInt(arguments[i+1]);
				else
					Logging.logFatal("No divisible by value provided!");
				i++;
			}
			
			//packing heuristic
			else if (arguments[i].equalsIgnoreCase(k_paramPlacementHeuristic) == true || arguments[i].equalsIgnoreCase(k_paramPlacementHeuristicShort) == true)
			{
				if (i+1 < arguments.length)
					options.m_packingHeuristic = parsePackingHeuristic(arguments[i+1]);
				else
					Logging.logFatal("No placement heuristic provided!");
				i++;
			}
			
			//line height
			else if (arguments[i].equalsIgnoreCase(k_paramLineHeight) == true || arguments[i].equalsIgnoreCase(k_paramLineHeightShort) == true)
			{
				if (i+1 < arguments.length)
					options.m_lineHeight = Integer.parseInt(arguments[i+1]);
				else
					Logging.logFatal("No line height provided!");
				i++;
			}
			
			//image compression
			else if (arguments[i].equalsIgnoreCase(k_paramImageCompression) == true || arguments[i].equalsIgnoreCase(k_paramImageCompressionShort) == true)
			{
				if (i+1 < arguments.length)
					options.m_imageCompression = arguments[i+1];
				else
					Logging.logFatal("No image compression provided!");
				i++;
			}
			
			//image format
			else if (arguments[i].equalsIgnoreCase(k_paramImageFormat) == true || arguments[i].equalsIgnoreCase(k_paramImageFormatShort) == true)
			{
				if (i+1 < arguments.length)
					options.m_imageFormat = arguments[i+1];
				else
					Logging.logFatal("No image format provided!");
				i++;
			}
			
			//image dither
			else if (arguments[i].equalsIgnoreCase(k_paramDitherImage) == true || arguments[i].equalsIgnoreCase(k_paramDitherImageShort) == true)
			{
				options.m_imageDither = true;
			}
			
			//image premultiply alpha
			else if (arguments[i].equalsIgnoreCase(k_paramDisablePremultipliedAlpha) == true || arguments[i].equalsIgnoreCase(k_paramDisablePremultipliedAlphaShort) == true)
			{
				options.m_imagePremultiplyAlpha = false;
			}
			
			//help
			else if (arguments[i].equalsIgnoreCase(k_paramHelp) == true || arguments[i].equalsIgnoreCase(k_paramHelpShort) == true)
			{
				printHelpText();
				return;
			}
			
			//failure
			else
			{
				Logging.logFatal("Invalid parameter found: " + arguments[i]);
			}
		}
		
		if (options.m_inputDirectoryPath.length() == 0 || options.m_outputFilePath.length() == 0)
		{
			Logging.logFatal("Must provide input directory and output file paths.");
		}
		
		CSFontBuilder tool = new CSFontBuilder();
		tool.Run(options);
		
		Logging.finish();
	}
	/**
	 * Parses a comma separated list of integers into an array.
	 *
	 * @author Ian Copland
	 * 
	 * @param A string containing a comma separated list of
	 * integers.
	 * 
	 * @return An array containing the list of integers.
	 */
	private static int[] parseIntegerArray(String in_commaSeparatedList)
	{
		int[] integers = null;
		
		try
		{
			String[] stringIntegers = in_commaSeparatedList.split(",");
			integers = new int[stringIntegers.length];
			for (int i = 0; i < stringIntegers.length; ++i)
			{
				integers[i] = Integer.parseInt(stringIntegers[i]);
			}
		}
		catch (Exception e)
		{
			Logging.logFatal(StringUtils.convertExceptionToString(e));
		}
		
		return integers;
	}
	/**
	 * Parses the packing heuristic.
	 *
	 * @author Ian Copland
	 * 
	 * @param Packing heuristic as a string.
	 * 
	 * @return The heuristic.
	 */
	private static PlacementHeuristic parsePackingHeuristic(String in_packingHeuristic)
	{
		if (in_packingHeuristic.equalsIgnoreCase(k_placementHeuristicTopLeft) == true)
		{
			return PlacementHeuristic.TOPLEFT;
		}
		else if (in_packingHeuristic.equalsIgnoreCase(k_placementHeuristicBottomRight) == true)
		{
			return PlacementHeuristic.BOTTOMRIGHT;
		}
		else
		{
			Logging.logFatal("Invalid packing heuristic: " + in_packingHeuristic);
			return PlacementHeuristic.TOPLEFT;
		}
	}
	/**
	 * Outputs help test for this tool. This will be displayed regardless of 
	 * the current logging level.
	 *
	 * @author Ian Copland
	 */
	private static void printHelpText()
	{
		Logging.setLoggingLevel(LoggingLevel.k_verbose);
		Logging.logVerbose("Usage: java -jar CSFontBuilder.jar " + k_paramInput + " <directory path> " + k_paramOutput + " <file path> [" + k_paramFixedWidth + " <width>] " +
				"[" + k_paramFixedHeight + " <height>] [" + k_paramValidWidths + " <w1,w2,w3...>] [" + k_paramValidHeights + " <h1,h2,h3...>] [" + k_paramMaxWidth + " <width>] " + 
				"[" + k_paramMaxHeight + " <height>] [" + k_paramDivisibleBy + "<number>] [" + k_paramPlacementHeuristic + " <heuristic>] [" + k_paramLineHeight + " <height>] " + 
				"[" + k_paramImageCompression + " <compression>] [" + k_paramImageCompression + " <compression>] [" + k_paramImageFormat + " <format>] [" + k_paramDitherImage + "] " +
				"[" + k_paramDisablePremultipliedAlpha + "] [" + Logging.k_paramLoggingLevel + " <level>] [" + k_paramHelp + "]");
		Logging.logVerbose("Parameters:");
		Logging.logVerbose(" " + k_paramInput + "(" + k_paramInputShort + "): The input directory path containing the character PNGs.");
		Logging.logVerbose(" " + k_paramOutput + "(" + k_paramOutputShort + "): The output font file path.");
		Logging.logVerbose(" " + k_paramFixedWidth + "(" + k_paramFixedWidthShort + "): [Optional] Forces the output font image to have the given width.");
		Logging.logVerbose(" " + k_paramFixedHeight + "(" + k_paramFixedHeightShort + "): [Optional] Forces the output font image to have the given height.");
		Logging.logVerbose(" " + k_paramValidWidths + "(" + k_paramValidWidthsShort + "): [Optional] A comma separated list of valid output widths.");
		Logging.logVerbose(" " + k_paramValidHeights + "(" + k_paramValidHeightsShort + "): [Optional] A comma separated list of valid output heights.");
		Logging.logVerbose(" " + k_paramMaxWidth + "(" + k_paramMaxWidthShort + "): [Optional] The maximum possible width for the output font image.");
		Logging.logVerbose(" " + k_paramMaxHeight + "(" + k_paramMaxHeightShort + "): [Optional] The maximum possible height for the output font image.");
		Logging.logVerbose(" " + k_paramDivisibleBy + "(" + k_paramDivisibleByShort + "): [Optional] The output image will dimensions divisible by the given value.");
		Logging.logVerbose(" " + k_paramPlacementHeuristic + "(" + k_paramPlacementHeuristicShort + "): [Optional] The heuristic to use when packing characters.");
		Logging.logVerbose(" " + k_paramLineHeight + "(" + k_paramLineHeightShort + "): [Optional] The height of a single line of text for the font.");
		Logging.logVerbose(" " + k_paramImageCompression + "(" + k_paramImageCompressionShort + "): [Optional] The compression used for the output font image.");
		Logging.logVerbose(" " + k_paramImageFormat + "(" + k_paramImageFormatShort + "): [Optional] The format of the output font image.");
		Logging.logVerbose(" " + k_paramDitherImage + "(" + k_paramDitherImageShort + "): [Optional] If set the output font image will be dithered.");
		Logging.logVerbose(" " + k_paramDisablePremultipliedAlpha + "(" + k_paramDisablePremultipliedAlphaShort + "): [Optional] If set the output font image will not have it's alpha premultiplied.");
		Logging.logVerbose(" " + Logging.k_paramLoggingLevel + "(" + Logging.k_paramLoggingLevelShort + "): [Optional] The level of messages to log.");
		Logging.logVerbose(" " + k_paramHelp + "(" + k_paramHelpShort + "): [Optional] Display this help message.");
		Logging.logVerbose("Placement Heuristics:");
		Logging.logVerbose(" " + k_placementHeuristicTopLeft + ": Place top left.");
		Logging.logVerbose(" " + k_placementHeuristicBottomRight + ": Place bottom right.");
		Logging.logVerbose("Image Formats:");
		Logging.logVerbose(" L8");
		Logging.logVerbose(" LA88");
		Logging.logVerbose(" RGB565");
		Logging.logVerbose(" RGBA4444");
		Logging.logVerbose(" RGB888");
		Logging.logVerbose(" RGBA8888");
		Logging.logVerbose("Compression Types:");
		Logging.logVerbose(" None");
		Logging.logVerbose(" Default");
		Logging.logVerbose("Logging Levels:");
		Logging.logVerbose(" " + Logging.k_loggingLevelNone + ": No logging.");
		Logging.logVerbose(" " + Logging.k_loggingLevelFatal + ": Only log fatal errors.");
		Logging.logVerbose(" " + Logging.k_loggingLevelError + ": Only log errors.");
		Logging.logVerbose(" " + Logging.k_loggingLevelWarning + ": Log errors and warnings.");
		Logging.logVerbose(" " + Logging.k_loggingLevelVerbose + ": Log all messages.");
	}
}
