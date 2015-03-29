#include "debugstream.h"
#include <Windows.h>

/**
 * ����DebugStream��streambuf
 */
class DebugStreambuf : public std::streambuf {
public:
	DebugStreambuf(int buf_size) : _buf(new char[buf_size + 1]) {
		setp(_buf, _buf + buf_size);
	}
	
	virtual ~DebugStreambuf() {
		delete[] pbase();
	}
	
	/**
	 * ��ΪDebugStream��ostream��������������EOF��
	 */
	virtual int_type underflow() {
		return traits_type::eof();
	}
	
	/**
	 * ͬ���������������������
	 */
	virtual int sync() {
		*pptr() = 0;
		OutputDebugString(pbase());
		setp(pbase(), epptr());
		return 0;
	}
	
	/**
	 * ��������������ʱ������
	 */
	virtual int_type overflow(int_type _Meta = traits_type::eof()) {
		if (sync() == -1) {
			return traits_type::eof();
		} else {
			if (!traits_type::eq_int_type(_Meta, traits_type::eof())) {
				sputc(traits_type::to_char_type(_Meta));
			}
			return traits_type::not_eof(_Meta);
		}		
	}
	
private:
	char* _buf;	
};

// --------------------------------------------------------------------------------------------

namespace utility {

DebugStream::DebugStream() : std::ostream(new DebugStreambuf(1024 * 4 - 4)) {
}

DebugStream::~DebugStream() {
	delete rdbuf();
}

DebugStream debug;

} // namespace utility