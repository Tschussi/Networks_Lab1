/*
 * layer2.c
 *
 *  Created on: Feb 19, 2017
 *      Author: Kevin Hames
 */

#include "nw_lab1.h"

buffer layer2(buffer t_buffer, char mode, int debug)
{
	buffer t_buffer_new;
	t_buffer_new.data = (char *) malloc(4800);
	t_buffer_new.B_count = t_buffer.B_count;
	int last_frame = t_buffer.B_count / MSS;
	int lf_count = t_buffer.B_count % MSS + 60 - 1; // data + hdr & trl bits = size of last frame
	int lf_b_count = lf_count; // used to store Byte count as 3 chars

	// Transmit Mode
	if(mode == 't')
	{
		// Debug: Prints incoming data from layer 3
		if(debug == 1)
		{
			printf("\nPre Layer 2 Transmit Function Test\n");
			for(int i=0; i<last_frame; i++)
			{
				printf("Frame %i: ", i);
				for(int j=0; j<100; j++)
				{
					printf("%c", *(t_buffer.data +j + i*100));
				}
				printf("\n");
			}
			printf("Frame %i: ", last_frame);
			for(int j=0; j<t_buffer_new.B_count % MSS + 40 -1; j++) // last frame (+40 for hdr)
			{
				printf("%c", *(t_buffer.data +j + last_frame*100));
			}
			printf("\n");
		}

		// Adds Layer 2 hdr and trlr to transmission buffer
		for(int i = 0; i<last_frame; i++)
		{
			for(int j=0; j<120;j++)
			{
				if(j==0) // STX
					*(t_buffer_new.data + i*120) = (char) 2;
				else if(j==1) // Byte Count
				{
					*(t_buffer_new.data + i*120 + 1) = '1';
					*(t_buffer_new.data + i*120 + 2) = '2';
					*(t_buffer_new.data + i*120 + 3) = '0';
					j = j + 2;
				}
				else if(j == 4) // ACK
					*(t_buffer_new.data + i*120 + j) = '0';
				else if (j == 5) // Seq#
					*(t_buffer_new.data + i*120 + j) = (char) (i%10 + 48);
				else if (j == 6) // LFrame
					*(t_buffer_new.data + i*120 + j) = '0';
				else if (j < 17) // RESVD
				{
					*(t_buffer_new.data + i*120 + j) = '0';
				}
				else if (j < 57) // L3H
					*(t_buffer_new.data + i*120 + j) = *(t_buffer.data + i*100 + j-17);
				else if (j<117) // DATA
				{
					*(t_buffer_new.data + i*120 + j) = *(t_buffer.data + i*100 + j-17);
				}
				else if (j < 119) // ChkSum
					*(t_buffer_new.data + i*120 + j) = '0';
				else	// ETX
					*(t_buffer_new.data + i*120 + j) = (char) 3;
			}
		}
		for(int j=0; j<lf_count;j++) // last frame special case
		{
		if(j==0) // STX
			*(t_buffer_new.data + last_frame*120) = (char) 2;
		else if(j==1) // Byte Count (changed to fix bug)
		{
			*(t_buffer_new.data + last_frame*120 + 1) = (char) (lf_b_count / 100 + 48);
			lf_b_count = lf_b_count % 100;
			*(t_buffer_new.data + last_frame*120 + 2) = (char) (lf_b_count / 10  + 48);
			*(t_buffer_new.data + last_frame*120 + 3) = (char) (lf_b_count % 10 + 48);
			j = j + 2;
		}
		else if(j == 4) // ACK
			*(t_buffer_new.data + last_frame*120 + j) = '0';
		else if (j == 5) // Seq#
			*(t_buffer_new.data + last_frame*120 + j) = (char) (last_frame%10 + 48);
		else if (j == 6) // LFrame
			*(t_buffer_new.data + last_frame*120 + j) = '1';
		else if (j < 17) // RESVD
		{
			*(t_buffer_new.data + last_frame*120 + j) = '0';
		}
		else if (j < 57) // L3H
			*(t_buffer_new.data + last_frame*120 + j) = *(t_buffer.data + last_frame*100 + j-17);
		else if (j< lf_count - 3) // DATA
			*(t_buffer_new.data + last_frame*120 + j) = *(t_buffer.data + last_frame*100 + j-17);
		else if (j < lf_count -1) // ChkSum
			*(t_buffer_new.data + last_frame*120 + j) = '0';
		else	// ETX
			*(t_buffer_new.data + last_frame*120 + j) = (char) 3;
		}

	// Debug: prints outgoing data from layer 2
	if(debug == 1)
	{
		printf("\nPost Layer 2 Transmit Function Test\n");
		for(int i=0; i<last_frame; i++)
		{
			printf("Frame %i: ", i);
			for(int j=0; j<120; j++)
			{
				printf("%c", *(t_buffer_new.data +j + i*120));
			}
			printf("\n");
		}
		printf("Frame %i: ", last_frame);
		for(int k=0; k < lf_count; k++)
		{
			printf("%c", *(t_buffer_new.data + k + last_frame*120));
		}
		printf("\n");
	}

	return t_buffer_new;
	}
	// Receiving Mode
	else if(mode == 'r')
	{

		// Debug: prints incoming data from layer 1
		if(debug == 1)
		{
			printf("\nPre Layer 2 Recieve Function Test\n");
			for(int i=0; i<last_frame; i++)
			{
				printf("Frame %i: ", i);
				for(int j=0; j<120; j++)
				{
					printf("%c", *(t_buffer.data +j + i*120));
				}
				printf("\n");
			}
			printf("Frame %i: ", last_frame);
			for(int k=0; k < lf_count; k++)
			{
				printf("%c", *(t_buffer.data + k + last_frame*120));
			}
			printf("\n");
		}

		// strip layer 2 hdr and trlr
		for(int i=0; i<last_frame; i++)
		{
			for(int j = 17; j < 117; j++)
			{
				*(t_buffer_new.data + i*100 + j-17) = *(t_buffer.data + i*120 + j);
			}
		}
		for(int j=17; j< lf_count-3; j++) // last frame special case
		{
			*(t_buffer_new.data + last_frame*100 + j-17) = *(t_buffer.data + last_frame*120 + j);
		}

		// Debug: Prints output of layer 2
		if(debug == 1)
		{
			printf("\nPost Layer 2 Receive Function Test\n");
			for(int i=0; i<last_frame; i++)
			{
				printf("Frame %i: ", i);
				for(int j=0; j<100; j++)
				{
					printf("%c", *(t_buffer_new.data +j + i*100));
				}
				printf("\n");
			}
			printf("Frame %i: ", last_frame);
			for(int j=0; j<t_buffer_new.B_count % MSS + 40 -1; j++) // last frame (+40 for hdr)
							{
								printf("%c", *(t_buffer_new.data +j + last_frame*100));
							}
							printf("\n");
		}

		return t_buffer_new;
	}
	else
		return t_buffer;
}
