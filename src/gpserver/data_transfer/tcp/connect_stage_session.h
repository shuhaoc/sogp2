#pragma once
#include "../transfer_protocol.h"

namespace gpserver {
namespace data_transfer {
namespace tcp {

/**
 * TCPת���Ự�����ӽ׶Σ�Ҳ�ǵ�һ�׶Ρ��������ͨ��new����������shared_ptr����
 * ת������ˣ�S��aync_acceptǰ��newһ��������󣬵�S�ɹ���������ת�������ߣ���
 * ��A�������ӣ�Ӧ�õ���start������ʼ�˻Ự������򡣻Ự����Ϊ������A��S����Ŀ
 * ���û����û�B����DestinationEndpoint��S����B��S��A�����������ӽ��ConnectResult��
 * �μ�../transfer_protocol.h�еĶ��塣
 * �����һ����ɣ��ᴴ���Ϳ�ʼһ��ת���׶λỰTransferStageSession��
 */
class ConnectStageSession : public boost::enable_shared_from_this<ConnectStageSession>, 
							private boost::noncopyable {
public:
	/**
	 * ���죬˫���tcp::socket���������ﴴ��
	 */
	ConnectStageSession(boost::asio::io_service& ios);
	
	/**
	 * ��ȡ�뷢�����û����ӵ�tcp::socket
	 */
	boost::asio::ip::tcp::socket& src_socket() { return *_src_socket; }

	/**
	 * aync_accept�ɹ��󣬲��ܿ�ʼ�Ự�е��첽����
	 */
	void start();

private:
	/**
	 * ����A���͵�B��DestinationEndpoint��ɣ���ʼ����B
	 */
	void recvDestEndpointHandler(const boost::system::error_code& error,
								 size_t bytes_transferred);

	/**
	 * ����B��ɣ���ʼ��A�������ӽ��
	 */
	void connectDestinationHandler(const boost::system::error_code& error);

	/**
	 * ��A�������ӽ����ɣ�����������һ��ת���׶λỰ
	 */
	void sendConnectResultHandler(const boost::system::error_code& error,
								  size_t bytes_transferred);

	boost::shared_ptr<boost::asio::ip::tcp::socket> _src_socket;
	boost::shared_ptr<boost::asio::ip::tcp::socket> _dest_socket;
	DestinationEndpoint _dest_endpoint;
	ConnectResult _connect_result;
};

} // namespace tcp
} // namespace data_transfer
} // namespace gpserver