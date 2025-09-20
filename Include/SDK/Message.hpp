#pragma once

namespace fb {

class MessageManager {
public:

private:
    /* 0x00 */ class MessageManagerImpl* m_impl;
    /* 0x08 */ bool m_owner;
}; /* Size: 0x10 */

}
