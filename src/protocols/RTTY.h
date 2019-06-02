#ifndef _RADIOLIB_RTTY_H
#define _RADIOLIB_RTTY_H

#include "TypeDef.h"
#include "PhysicalLayer.h"

#define ITA2_FIGS                                     0x1B
#define ITA2_LTRS                                     0x1F
#define ITA2_LENGTH                                   32

// ITA2 character table: - position in array corresponds to 5-bit ITA2 code
//                       - characters to the left are in letters shift, characters to the right in figures shift
//                       - characters marked 0x7F do not have ASCII equivalent
static const char ITA2Table[ITA2_LENGTH][2] PROGMEM = {{'\0', '\0'}, {'E', '3'}, {'\n', '\n'}, {'A', '-'}, {' ', ' '}, {'S', '\''}, {'I', '8'}, {'U', '7'},
                                                       {'\r', '\r'}, {'D', 0x05}, {'R', '4'}, {'J', '\a'}, {'N', ','}, {'F', '!'}, {'C', ':'}, {'K', '('},
                                                       {'T', '5'}, {'Z', '+'}, {'L', ')'}, {'W', '2'}, {'H', 0x7F}, {'Y', '6'}, {'P', '0'}, {'Q', '1'},
                                                       {'O', '9'}, {'B', '?'}, {'G', '&'}, {0x7F, 0x7F}, {'M', '.'}, {'X', '/'}, {'V', ';'}, {0x7F, 0x7F}};

/*!
  \class ITA2String

  \brief ITA2-encoded string.
*/
class ITA2String {
  public:
    /*!
      \brief Default single-character constructor.

      \param c ASCII-encoded character to encode as ITA2.
    */
    ITA2String(char c);

    /*!
      \brief Default string constructor.

      \param str ASCII-encoded string to encode as ITA2.
    */
    ITA2String(const char* str);

    /*!
      \brief Default destructor.
    */
    ~ITA2String();

    /*!
      \brief Gets the length of the ITA2 string. This number is not the same as the length of ASCII-encoded string!

      \returns Length of ITA2-encoded string.
    */
    size_t length();

    /*!
      \brief Gets the ITA2 representation of the ASCII string set in constructor.

      \returns Pointer to dynamically allocated array, which contains ITA2-encoded bytes.
      It is the caller's responsibility to deallocate this memory!
    */
    uint8_t* byteArr();

  private:
    char* _str;
    size_t _len;
    size_t _ita2Len;

    uint16_t getBits(char c);
};

// supported encoding schemes
#define ASCII                                         0
#define ASCII_EXTENDED                                1
#define ITA2                                          2

/*!
  \class RTTYClient

  \brief Client for RTTY communication. The public interface is the same as Arduino Serial.
*/
class RTTYClient {
  public:
    /*!
      \brief Default constructor.

      \param phy Pointer to the wireless module providing PhysicalLayer communication.
    */
    RTTYClient(PhysicalLayer* phy);

    // basic methods

    /*!
      \brief Initialization method.

      \param base Base (space) RF frequency to be used in MHz.

      \param shift Frequency shift between mark and space in Hz.

      \param rate Baud rate to be used during transmission.

      \param encoding Encoding to be used. Defaults to ASCII.

      \param stopBits Number of stop bits to be used.

      \returns \ref status_codes
    */
    int16_t begin(float base, uint32_t shift, uint16_t rate, uint8_t encoding = ASCII, uint8_t stopBits = 1);

    /*!
      \brief Send out idle condition (RF tone at mark frequency).
    */
    void idle();

    size_t write(const char* str);
    size_t write(uint8_t* buff, size_t len);
    size_t write(uint8_t b);

    size_t print(__FlashStringHelper*);
    size_t print(ITA2String &);
    size_t print(const String &);
    size_t print(const char[]);
    size_t print(char);
    size_t print(unsigned char, int = DEC);
    size_t print(int, int = DEC);
    size_t print(unsigned int, int = DEC);
    size_t print(long, int = DEC);
    size_t print(unsigned long, int = DEC);
    size_t print(double, int = 2);

    size_t println(void);
    size_t println(__FlashStringHelper*);
    size_t println(ITA2String &);
    size_t println(const String &s);
    size_t println(const char[]);
    size_t println(char);
    size_t println(unsigned char, int = DEC);
    size_t println(int, int = DEC);
    size_t println(unsigned int, int = DEC);
    size_t println(long, int = DEC);
    size_t println(unsigned long, int = DEC);
    size_t println(double, int = 2);

  private:
    PhysicalLayer* _phy;

    uint8_t _encoding;
    uint32_t _base;
    uint32_t _shift;
    uint16_t _bitDuration;
    uint8_t _dataBits;
    uint8_t _stopBits;

    void mark();
    void space();

    size_t printNumber(unsigned long, uint8_t);
    size_t printFloat(double, uint8_t);
};

#endif
