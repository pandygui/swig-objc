/* -----------------------------------------------------------------------------
 * This file is part of SWIG, which is licensed as a whole under version 3 
 * (or any later version) of the GNU General Public License. Some additional
 * terms also apply to certain portions of SWIG. The full details of the SWIG
 * license and copyrights can be found in the LICENSE and COPYRIGHT files
 * included with the SWIG source code as distributed by the SWIG developers
 * and at http://www.swig.org/legal.html.
 *
 * PyDocConverter.h
 *
 * Module to return documentation for nodes formatted for PyDoc
 * ----------------------------------------------------------------------------- */

#ifndef PYDOCCONVERTER_H_
#define PYDOCCONVERTER_H_

#include <list>
#include <string>
#include "swig.h"
#include "DoxygenEntity.h"
#include "DoxygenTranslator.h"

#define DOC_STRING_LENGTH	  64	// characters per line allowed
#define DOC_PARAM_STRING_LENGTH	  30	// characters reserved for param name / type

class PyDocConverter : public DoxygenTranslator {
public:
  PyDocConverter();
  String *makeDocumentation(Node *node);

protected:
  /*
   * Format the doxygen comment relating to a function or method parameter
   * @param node The parse tree node that the parameter relates to.
   * @param doxygenEntity The doxygen entity relating to the parameter docstring.
   */
  std::string formatParam(Node *node, DoxygenEntity & doxygenEntity);

  /*
   * Format the doxygen comment relating to the return value for a method or function.
   * @param node The parse tree node that the parameter relates to.
   * @param doxygenEntity The doxygen entity relating to the parameter docstring.
   */
  std::string formatReturnDescription(Node *node, DoxygenEntity & doxygenEntity);

  /*
   * Format a string so it is justified and split over several lines 
   * not exeeding a given length.
   * @param unformattedLine The string to justify and split.
   * @param indent An optional number of spaces to indent each line by.
   * @param maxWidth The approximate maximum line width.
   */
  std::string justifyString(std::string unformattedLine, int indent = 0, int maxWidth = DOC_STRING_LENGTH);

  std::string translateSubtree(DoxygenEntity & doxygenEntity);
  void translateEntity(DoxygenEntity & doxyEntity, std::string &translatedComment);

  /*
   * Typedef for the function that handles one tag
   * arg - some string argument to easily pass it through lookup table
   */
  typedef void (PyDocConverter::*tagHandler)(DoxygenEntity &tag,
      std::string &translatedComment, std::string &arg);

  /*
   * Wrap the command data with the some string
   * arg - string to wrap with, like '_' or '*'
   */
  void handleTagWrap(DoxygenEntity &tag, std::string &translatedComment, std::string &arg);
  /*
   * Just prints new line
   */
  void handleNewLine(DoxygenEntity &tag, std::string &translatedComment, std::string &arg);
  /*
   * Print the name of tag to the output, used for escape-commands
   */
  void handleTagChar(DoxygenEntity &tag, std::string &translatedComment, std::string &arg);
  /*
   * Print only the content and strip original tag
   */
  void handleParagraph(DoxygenEntity &tag, std::string &translatedComment, std::string &arg);
  /*
   * Print only data part of code
   */
  void handlePlainString(DoxygenEntity &tag, std::string &translatedComment, std::string &arg);
  /*
   * Print the if-elseif-else-endif section
   */
  void handleTagIf(DoxygenEntity &tag, std::string &translatedComment, std::string &arg);
  /*
   * Prints the specified message, than the contents of the tag
   * arg - message
   */
  void handleTagMessage(DoxygenEntity &tag, std::string &translatedComment, std::string &arg);
  /*
   * Insert 'Image: ...'
   */
  void handleTagImage(DoxygenEntity &tag, std::string &translatedComment, std::string &arg);
  /*
   * Insert 'Title: ...'
   */
  void handleTagPar(DoxygenEntity &tag, std::string &translatedComment, std::string &arg);

  /*
   * Utility method to generate a diving line for a documentation string.
   */
  std::string generateDivider();

private:
  bool debug;
  // temporary thing, should be refactored somehow
  Node *currentNode;
  // this contains the handler pointer and one string argument
  static std::map<std::string, std::pair<tagHandler, std::string> > tagHandlers;
  // this cointains the sectins titiles, like 'Arguments:' or 'Notes:', that are printed only once
  static std::map<std::string, std::string> sectionTitles;
  void fillStaticTables();
};

#endif
