#ifndef CALLBACK_H
#define CALLBACK_H

#include <functional>
#include <condition_variable>

template<typename ret, typename ... args>
class Callback {
	
	std::mutex m_mutex;
	std::condition_variable m_cv;
	
	std::function<void(args...)> m_sender;
	std::function<void(ret &)> m_receiver;
	
	std::string m_name;
	
public:
	
	Callback(const std::string & name = "callback") :
		m_sender([](args...){}),
		m_receiver([](ret &){}),
		m_name(name) {}
		
	Callback(const std::function<void(args...)> & sender,
			 const std::function<void(ret &)> & receiver,
			 const std::string & name = "callback") :
	m_sender(sender),
	m_receiver(receiver),
	m_name(name) {}
	
	~Callback() = default;
	
	inline void setName(const std::string & name) { m_name = name; }
	inline const std::string & getName() const { return m_name; }
	
	inline void setSender(const std::function<void(args...)> & sender) { m_sender = sender; }
	inline void setReceiver(const std::function<void(ret &)> & receiver) { m_receiver = receiver; }
	
	inline void send(args... a) {
		m_sender(a...);
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait(lock);
	}
	
	inline void receive(ret & value) {
		m_receiver(value);
		m_cv.notify_one();
	}
	
};

#endif //CALLBACK_H
