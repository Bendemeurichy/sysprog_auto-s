#ifndef SYSPROG_BITHELPER_H
#define SYSPROG_BITHELPER_H

#include <cstdint>
#include <functional>
#include <memory>
#include <assert.h>

/**
 * Een lambda om bytes naar iets door te geven.
 *
 * Example:
 *    ByteSink sink = [](uint8_t byte){ printf("sinking: %X\n", byte); };
 *    sink(3);
 *    sink(0xF5);
 *
 * prints:
 *    sinking: 3
 *    sinking: F5
 * */
typedef std::function<void(uint8_t)> ByteSink;

/**
 * Een lamda om bytes te ontvangen.
 *
 * Example:
 *    uint8_t a = 4;
 *    ByteSource source = [&a](){ a += 3; return a; };
 *    printf("Got: %X\n", source());
 *    printf("Got: %X\n", source());
 *
 * Prints:
 *    Got: 7
 *    Got: A
 */
typedef std::function<uint8_t()> ByteSource;

/**
 * BitWriter laat gebruikers toe 1 of meerdere bits per keer toe te voegen, en op het einde de resulterende bytes op te vragen.
 *
 * Voorbeeld:
 *    uint8_t a = 6;
 *    uint8_t b = 0x9;    //also same result for 0xF9
 *    uint16_t c = 0xB05; //also same result for 0xFB05
 *    BitWriter<10> bw;
 *
 *    bw.writeBits(a, 3);
 *    printf("bytes=%lu\n", bw.getByteCount());
 *    bw.writeBits(b, 4);
 *    printf("bytes=%lu\n", bw.getByteCount());
 *    bw.writeBits(c, 12);
 *    printf("bytes=%lu\n", bw.getByteCount());
 *
 *    ByteSink sink = [](uint8_t byte){ printf("sinking: %X\n", byte); };
 *    bw.toSink(sink);
 *
 * Prints:
 *    bytes=1
 *    bytes=1
 *    bytes=3
 *    sinking: D3
 *    sinking: 60
 *    sinking: A0
 *
 * @tparam max_bytes Een template parameter die het maximum aantal bytes dat deze BitWriter ooit moet opslaan aangeeft.
 */
template<int max_bytes>
class BitWriter {
private:
    //TODO: private velden
public:
    BitWriter();
    /**
     * Slaat de opgegeven bits op. De bits worden big endian opgeslaan (enkel relevant bij meer dan 8 bits).
     * De "source" wordt gebruikt als bron van de bits. De "bitCount" laagste bits worden gebruikt.
     * @tparam source_t het type van de source parameter
     * @param source data met minstens het toe te voegen aantal bits
     * @param bitCount het aantal bits dat toe te voegen is
     */
    template<class source_t>
    void writeBits(source_t source, size_t bitCount);

    /**
     * @return het aantal bytes dat nodig is om alle bits op te slaan. Er wordt naar boven afgerond.
     */
    [[nodiscard]] size_t getByteCount() const;
    
    /**
     * Haalt binnen alle ontvangen bits, 1 specifieke byte op.
     * @param index de positie van de gevraagde byte (moet kleiner dan "getByteCount()" zijn)
     * @return de gevraagde byte
     */
    [[nodiscard]] uint8_t getByte(size_t index) const;
    
    /**
     * Schrijft alle tot nu toe ontvangen bits weg naar de byteSink.
     * @param byteSink een lambda die de tot nu toe ontvangen bits, byte per byte zal ontvangen.
     * @return het aantal bytes dat doorgegeven is aan de sink. Dit is gelijk aan "getByteCount()"
     */
    size_t toSink(const ByteSink &byteSink) const;
};

/**
 * BitReaders leest bytes van een bron, en laat vervolgens gebruikers de bits (1 voor 1 of in in groepen) uitlezen.
 *
 * Voorbeeld:
 *    uint8_t a[] = { 0xD3, 0x60, 0xA0, 0xFF };
 *    uint i = 0;
 *    ByteSource source = [&i, a](){ return a[i++]; };
 *
 *    BitReader<4> br(source);
 *    uint8_t one = br.readBits<uint8_t>(3);
 *    uint8_t two = br.readBits<uint8_t>(4);
 *    uint16_t three = br.readBits<uint16_t>(12);
 *    printf(" 3 bits: %X\n", one);
 *    printf(" 4 bits: %X\n", two);
 *    printf("12 bits: %X\n", three);
 *    printf("bits read: %lu\n", br.getBitsRead());
 *    printf("bytes read: %lu\n", br.getBytesRead());
 *
 * Output:
 *     3 bits: 6
 *     4 bits: 9
 *    12 bits: B05
 *    bits read: 19
 *    bytes read: 3
 *
 * @tparam max_bytes template parameter die het maximum aantal bytes dat deze BitReader kan lezen aangeeft.
 * Opmerking: een zeer goede implementatie kan generiek zijn (= geen limiet op aantal gelezen bytes), waardoor deze parameter weg gelaten mag worden.
 */
template<int max_bytes>
class BitReader {
private:
    //TODO: private velden
public:
    /**
     * Constructor, met een byteSource als parameter
     * @param byteSource een lambda waaruit de bytes gelezen worden, wanneer ze nodig zijn. (er worden nooit meer bytes gelezen dan nodig)
     */
    explicit BitReader(ByteSource byteSource);

    /**
     * Leest het opgegeven aantal bits, en geeft het resulterende getal terug.
     * Als het type van het resultaat meer bits bevat dan gevraagd, zijn de hoogste bits 0.
     * Dit is logisch by een unsigned type.
     *
     * @tparam target_t type van resultaat
     * @param bitCount aantal te lezen bits
     * @return
     */
    template<class target_t>
    target_t readBits(size_t bitCount);

    /**
     * @return het huidige aantal gelezen bits (= de som van het "bitCount" argument van alle voorgaande "readBits" calls)
     */
    [[nodiscard]] size_t getBitsRead() const;
    /**
     * @return het huidige aantal gelezen bytes. (Afgeleid van "getBitsRead". Naar boven afgerond.)
     */
    [[nodiscard]] size_t getBytesRead() const;
};

//TODO: Schrijf grondige tests voor deze klassen

//TODO: implementeer alle methoden van BitWriter en BitReader
//      (in deze header ipv cpp file, omwille van de templates!)


template<int max_bytes>
BitWriter<max_bytes>::BitWriter() {
    //TODO
}

template<int max_bytes>
template<class source_t>
void BitWriter<max_bytes>::writeBits(source_t source, size_t bitCount) {
    //TODO
}

template<int max_bytes>
size_t BitWriter<max_bytes>::getByteCount() const {
    return 0; //TODO
}

template<int max_bytes>
uint8_t BitWriter<max_bytes>::getByte(size_t index) const {
    return 0; //TODO
}

template<int max_bytes>
size_t BitWriter<max_bytes>::toSink(const ByteSink &byteSink) const {
    return 0; //TODO
}


template<int max_bytes>
BitReader<max_bytes>::BitReader(ByteSource byteSource) {
    //TODO
}

template<int max_bytes>
template<class target_t>
target_t BitReader<max_bytes>::readBits(size_t bitCount) {
    return (target_t)0; //TODO
}

template<int max_bytes>
size_t BitReader<max_bytes>::getBytesRead() const {
    return (size_t)0; //TODO
}

template<int max_bytes>
size_t BitReader<max_bytes>::getBitsRead() const {
    return (size_t)0; //TODO
}

#endif //SYSPROG_BITHELPER_H