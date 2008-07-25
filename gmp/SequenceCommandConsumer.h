#ifndef JMSMESSAGECONSUMER_H_
#define JMSMESSAGECONSUMER_H_

#include <activemq/util/Config.h>
#include <decaf/util/concurrent/CountDownLatch.h>

#include <cms/MessageListener.h>
#include <cms/Session.h>

#include <giapi/giapi.h>
#include <giapi/SequenceCommandHandler.h>
#include <giapi/HandlerResponse.h>

#include <log4cxx/logger.h>

using namespace giapi;

namespace gmp {

using namespace cms;
using namespace decaf::util::concurrent;

/**
 * The Sequence Command Consumer is a JMS Listener in charge
 * of receiving and processing sequence commands. Whenever
 * a sequence command is delivered through the GMP, an instance
 * of this class registered for that particular sequence command
 * will react, processing the sequence command and invoking 
 * the appropriate SequenceCommandHandler specified through
 * the CommandUtil::subscribeSequenceCommand() method in the GIAPI
 * 
 * @see CommandUtil::subscribeSequenceCommand()
 */
class SequenceCommandConsumer : public MessageListener {

public:
	/**
	 * Constructor. The arguments specifiy what sequence command and
	 * activities this consumer will take care of, and the client
	 * sequence command handler that needs to be invoked whenever
	 * the corresponding sequence command is received. 
	 * 
	 * @param id the command::SequenceCommand this consumer will process
	 * @param activities the set of Activity elements (represented by 
	 * the enumerated type, command::ActivitySet) that this consumer
	 * will process.
	 * @param handler The pSequenceCommandHandler, a smart pointer containing
	 * the client implementation of a SequenceCommandHandler object that will
	 * be invoked when this consumer receives the selected SequenceCommand and
	 * Activity.
	 */
	SequenceCommandConsumer(command::SequenceCommand id,
			command::ActivitySet activities, pSequenceCommandHandler handler);
	
	/**
	 * Destructor. Cleans up all the resources instantiated by this consumer
	 */
	virtual ~SequenceCommandConsumer();
	
	/**
	 * Invoked by the JMS whenever a new message is received
	 */
	virtual void onMessage(const Message* message);

private:
	/**
	 * Logging facility
	 */
	static log4cxx::LoggerPtr logger;

	/**
	 * A mutex, used to know when the consumer is ready to process messages
	 */
	CountDownLatch latch;

	/** 
	 * The JMS Session associated to this consumer. Each consumer has its
	 * own session, since they need to operate in different execution 
	 * threads
	 */
	Session* _session;
	
	/**
	 * The virtual channel from where this consumer will get the messages.
	 * The destination is obtained based on the sequence command we have
	 * interest on
	 */
	Destination* _destination;
	
	/**
	 * The consumer instance for this object. Each consumer runs on its 
	 * own JMS Session
	 */
	MessageConsumer* _consumer;

	/**
	 * The handler to be invoked when a sequence command is received
	 */
	pSequenceCommandHandler _handler;

	/**
	 * The sequence command that is handled by this consumer
	 */
	command::SequenceCommand _sequenceCommand;

	/**
	 * A private method to build a message to provide the  reply
	 * when a sequence command is received. This method might be
	 * moved to an utility class later.
	 */
	Message * _buildReply(pHandlerResponse response);

	/**
	 * Close and destroy associated JMS resources used by this consumer
	 */
	void cleanup();

};

}

#endif /*MESSAGECONSUMER_H_*/
