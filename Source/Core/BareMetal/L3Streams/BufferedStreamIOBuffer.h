/**
 * @file BufferedStreamIOBuffer.h
 * @brief Header file for class BufferedStreamIOBuffer
 * @date 02/10/2015
 * @author Giuseppe Ferrò
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This header file contains the declaration of the class BufferedStreamIOBuffer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BUFFEREDSTREAMIOBUFFER_H_
#define BUFFEREDSTREAMIOBUFFER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "TimeoutType.h"
#include "IOBuffer.h"
#include "RawStream.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief BufferedStreamIOBuffer class.
 *
 * @details This class inherits from IOBuffer specializing NoMoreSpaceToWrite and NoMoreDataToRead functions accordingly
 * to the buffered stream requirements. In particular NoMoreSpaceToWrite acts as a flush on the stream and
 * NoMoreDataToRead refills this buffer from the stream. Moreover also the Resync function is implemented
 * due to adjust the stream position after a buffered read or write operation.
 *
 * @details Since this buffers has to read and write on the associated stream, in the constructor must be passed a pointer to the associated stream.
 */
class BufferedStreamIOBuffer: public IOBuffer {

public:

    /**
     * @brief Default constructor.
     */
    BufferedStreamIOBuffer();

    /**
     * @brief Constructor.
     * @param[in] s is a pointer to the stream which uses this buffer.
     * @param[in] msecTimeout is the desired timeout for read-write operations.
     */
    BufferedStreamIOBuffer(RawStream * const s,
                           const TimeoutType msecTimeout = TTDefault);

    /**
     * @brief User friendly function which simply calls NoMoreDataToRead.
     * @return the NoMoreDataToRead return.
     */
    inline bool Refill();

    /**
     * @brief User friendly function which simply calls NoMoreSpaceToWrite.
     * @return the NoMoreSpaceToWrite return.
     */
    inline bool Flush();

    /**
     * @brief Adjusts the position of the stream.
     * @details This function is called from the stream after a read operation because the position was shifted
     * forward (+bufferSize) because of the refill. Calls BufferedStream::Seek moving the cursor back (-UsedAmountLeft).
     *
     * @return false if the stream seek fails.
     */
    virtual bool Resync();

    /**
     * @brief Allocates or reallocate memory to the desired size.
     * @param[in] size is the desired size for the buffer.
     * @return false in case of allocations errors.
     */
    bool SetBufferSize(const uint32 size);

protected:

    /**
     * @brief Refills the buffer reading from the stream.
     * @details Empties the buffer, calls BufferedStream::UnBufferedRead with size = MaxUsableAmount.
     * In case of stream read error empties the buffer and returns false.
     *
     * @return false if the buffer is null or in case of stream read error.
     */
    virtual bool NoMoreDataToRead();

    /**
     * @brief Flushes the buffer writing on the stream.
     * @return false if the buffer is null or in case of stream write error.
     */
    virtual bool NoMoreSpaceToWrite();

private:
    /**
     * The stream that uses this buffer.
     */
    RawStream *stream;

    /**
     * The timeout for read and write operations.
     */
    TimeoutType timeout;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

bool BufferedStreamIOBuffer::Refill() {
    return NoMoreDataToRead();
}

bool BufferedStreamIOBuffer::Flush() {
    return NoMoreSpaceToWrite();
}

}
#endif /* BUFFEREDSTREAMIOBUFFER_H_ */

