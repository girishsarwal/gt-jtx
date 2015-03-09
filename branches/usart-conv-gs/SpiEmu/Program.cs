using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SpiEmu
{
    class Program
    {
        const int MAX_CHANNELS = 16;
        static byte currentOpcode = 0;
        static byte currentHiByte = 0;
        static byte currentLoByte = 0;
        static Int16 currentData = 0;
        
        static short[] ppm = new short[MAX_CHANNELS]
        {
            100,
            200,
            300,
            400,
            500,
            600,
            700,
            800,
            900,
            1000,
            1100,
            1200,
            1300,
            1400,
            1500,
            1600,
        };
        static short[] calibrationUpper = new short[MAX_CHANNELS];
        static short[] calibrationLower = new short[MAX_CHANNELS];
        static byte[] trims = new byte[MAX_CHANNELS];

        static SPISTATE state;

        enum SPISTATE
        {
            SYNC = 0x00,
            WAIT_HIBYTE = 0x01,
            WAIT_LOBYTE = 0x02,
        }

        enum OPCODE
            : byte
        {
            NOP = 0x00,
            SETTUP = 0x01,
            SETTDN = 0x02,
            GETT = 0x03,
            SETREV = 0x04,
            GETREV = 0x05,
            SCUP = 0x06,
            GCUP = 0x07,
            SCDN = 0x08,
            GCDN = 0x09,
            SPPMLEN = 0x0A,
            GPPMLEN = 0x0B,
            SPPMICL = 0x0C,
            GPPMICL = 0x0D,
            SSTIM = 0x0E,
            GSTIM = 0x0F,
            GCV = 0x10,
            SYNC = 0xFF,

        };
        static void help()
        {
            Console.WriteLine("SPI Emulator for gt-jtx comms");
            Console.WriteLine("-----------------------------");
            Console.WriteLine("THERE IS ***NO ACTUAL SPI CONNECTION GOING ON***.\n");
            Console.WriteLine("This should only be used to understand the gt-jtx comms protocol.");
            Console.WriteLine("The comm protocol uses three byte SPI commands");
            Console.WriteLine("The first byte is the opcode e.g. 10");
            Console.WriteLine("The second byte (HIBYTE) is data0 for the opcode. Any identification must be passed in this byte");
            Console.WriteLine("The third byte (LOBYTE) is data 1 for 16 bit output. Final values will be returned in the combination of 2nd and 3rd byte");
            Console.WriteLine("Type \"list\" to list all opcodes");
            Console.WriteLine("Type \"help\" to show this message");
            Console.WriteLine("Type \"quit\" to quit");
        }
        static void listOpcodes()
        {
            foreach (OPCODE item in Enum.GetValues(typeof(OPCODE)))
            {
                Console.WriteLine(String.Format("{0:x2} - {1}", (byte)item, item));
            }
        }
        static void Main(string[] args)
        {
            string input = String.Empty;
            help();
            bool bRunning = true;
            while (bRunning)
            {
                try
                {
                    switch (state)
                    {
                        case SPISTATE.SYNC:
                            Console.Write("\n>");
                            input = Console.ReadLine();
                            if (input == "quit") { bRunning = false; break; }
                            if (input == "help") { help(); break; }
                            if (input == "list") { listOpcodes(); break; }
                            OPCODE opcode = (OPCODE)Byte.Parse(input, System.Globalization.NumberStyles.HexNumber);
                            string command = Enum.GetName(typeof(OPCODE), opcode);
                            if (command == null)
                            {
                                Console.WriteLine("Command not implemented!");
                                continue;
                            }
                            Console.WriteLine(command);
                            Console.WriteLine(sendSPIByte((byte)opcode));
                            break;
                        case SPISTATE.WAIT_HIBYTE:
                            Console.Write("\n>HIBYTE>");
                            input = Console.ReadLine();
                            if (input == "quit") { bRunning = false; break; }
                            if (input == "help") { help(); break; }
                            if (input == "list") { listOpcodes(); break; }
                            byte hibyte = Byte.Parse(input, System.Globalization.NumberStyles.HexNumber);
                            Console.WriteLine(String.Format("{0:x2}", sendSPIByte(hibyte)));
                            break;
                        case SPISTATE.WAIT_LOBYTE:
                            Console.Write("\n>LOBYTE>");
                            input = Console.ReadLine();
                            if (input == "quit") { bRunning = false; break; }
                            if (input == "help") { help(); break; }
                            if (input == "list") { listOpcodes(); break; }
                            byte lobyte = Byte.Parse(input, System.Globalization.NumberStyles.HexNumber);
                            Console.WriteLine(String.Format("{0:x2}", sendSPIByte(lobyte)));

                            Console.WriteLine(String.Format("Complete data for opcode {0:x2} is {1:x4}", currentOpcode, currentData));
                            break;
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine("Not Understood!");
                }
            }
        }

        static short sendSPIByte(byte inp)
        {
            if((OPCODE)inp == OPCODE.SYNC)
            {
                return 0;
            }
            switch (state)
            {
                case SPISTATE.SYNC:
                    currentOpcode = inp;
                    state = SPISTATE.WAIT_HIBYTE;
                    return 0;
                case SPISTATE.WAIT_HIBYTE:
                    switch ((OPCODE)currentOpcode)
                        {
                            case OPCODE.SYNC:
                                state = SPISTATE.SYNC;
                                break;
                            case OPCODE.GCV:
                                currentData = (short)(ppm[(inp >> 4)]);
                                break;
                        }
                    state = SPISTATE.WAIT_LOBYTE;
                    currentHiByte = (byte)(currentData >> 8);
                    return currentHiByte;
                case SPISTATE.WAIT_LOBYTE:
                    currentLoByte = (byte)(currentData);
                    state = SPISTATE.SYNC;
                    return currentLoByte;
            }
            return 0;
        }
    }
}
