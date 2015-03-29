#pragma once

namespace gpserver {
namespace data_transfer {
namespace tcp {

/**
 * TCPת���Ự������ת���׶Σ�Ҳ�ǵڶ��׶Σ�ʵ�������û�֮�䵥���ת�����������
 * ͨ��new����������shared_ptr����������ͬʱ����һ�ԡ�
 * ������ˣ�S���������û�����Դ��A��Ŀ�귽B�����Ѿ���������֮�󣬴���A���ӵ�tcp::socket
 * ���ܵ����ݣ�Ȼ��ԭ�ⲻ���ط��͸�B��ͬʱ���Լ���Ӧ����һ������ʵ�ַ����ת����
 * ���κ�һ�˳�����ر�����tcp::socket��ͬʱ���Լ���Ӧ����һ������
 * �������ӱ��رն�������������ת����������
 */
class TransferStageSession : public boost::enable_shared_from_this<TransferStageSession>,
							 private boost::noncopyable {
public:
	/**
	 * ���죬����ʼ�����ݳ�Ա
	 */
	TransferStageSession(boost::shared_ptr<boost::asio::ip::tcp::socket> src_socket,
						 boost::shared_ptr<boost::asio::ip::tcp::socket> dest_socket);
						 
	/**
	 * �������ر�����tcp::socket
	 */
	~TransferStageSession();

	/**
	 * ��ʼ�ӷ��ͷ��첽��������
	 */
	void start();
		
private:
	/**
	 * ����Դ������������ɣ���ʼ���͸�Ŀ�귽
	 */
	void recvSrcDataHandler(const boost::system::error_code& error,
							size_t bytes_transferred);
						 
	/**
	 * ��Ŀ�귽������ɣ���������Դ����������
	 */
	void sendDestDataHandler(const boost::system::error_code& error,
							size_t bytes_transferred);						 
						 			
	boost::scoped_array<char> _buf;
	boost::shared_ptr<boost::asio::ip::tcp::socket> _src_socket;
	boost::shared_ptr<boost::asio::ip::tcp::socket> _dest_socket;
};		
		
} // namespace tcp
} // namespace data_transfer
} // namespace gpserver