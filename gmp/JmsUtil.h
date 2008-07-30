#ifndef JMSUTIL_H_
#define JMSUTIL_H_

#include <string>

#include <log4cxx/logger.h>

#include <giapi/giapi.h>
#include <giapi/HandlerResponse.h>

#include <util/giapiMaps.h>

using namespace giapi;
namespace gmp {

/**
 * This class contains static methods to perform translations between
 * the messaging system representation to actual objects and types  
 * used by the GIAPI
 */
class JmsUtil {

	/**
	 * Logging facility
	 */
	static log4cxx::LoggerPtr logger;

public:

	/**
	 * Get the messaging system topic from where the given 
	 * sequence command will be received. Clients use this
	 * topic to register when interested to receive a 
	 * given sequence command 
	 */
	static std::string getTopic(command::SequenceCommand id);

	/**
	 * Returns the Activity enumerated element associated to the
	 * string specified.
	 //TODO: This method should throw an Exception if the string can't be converted  
	 */
	static command::Activity getActivity(const std::string & string);

	/**
	 * Returns the string representation of the handler response provided
	 * as an argument. 
	 */
	static std::string getHandlerResponse(pHandlerResponse response);

	virtual ~JmsUtil();
private:

	/**
	 * Dictionary to map strings to Activity enumerated types
	 */
	StringActionIdMap activityMap;
	
	/**
	 * Dictionary to map HandlerResponse objects to Strings
	 */
	ResponseStringMap handlerResponseMap;

	/**
	 * Dictionary to map Sequence Commands to Strings
	 */
	SequenceCommandStringMap sequenceCommandMap;
	
	/**
	 * Private constructor preventing client instantiation of this class
	 */
	JmsUtil();

	/**
	 * Get the unique instance of the Jms Util
	 * 
	 * @return The JmsUtil singleton object
	 */
	static JmsUtil & Instance();
	
	/**
	 * The singleton instance of this utility class. 
	 * Private since it's used internally only, through the
	 * static methods
	 */
	static std::auto_ptr<JmsUtil> INSTANCE;
};

}

#endif /*JMSUTIL_H_*/