module LangParser( parseGlueLang ) where

import Text.Parsec
import Text.Parsec.String
import LangStructure

parseGlueLang :: String -> Script
parseGlueLang str = case parse code "" str of
                          Right scr    -> scr 
                          Left err     -> Err ( show err )

code = do is <- many langImport
          many blankLine
          fs <- many1 (try(langFunc) <|> try(langProc))
          return $ Script is fs


langImport = do string "import "
                p <- langWord
                string "as "
                a <- langWord
                char '\n'
                return $ Import p a

langProc      = do string "proc "
                   nm <- langWord
	           args <- many langWord
                   many $ oneOf " \t"
                   colonReturn
                   sb <- many1 (try(ifInlineCmd) <|> try(subInlineCmd))
                   many $ char '\n'
                   many blankLine
                   return $ Proc nm (zip [1..] args) sb

subInlineCmd = do lns <- many1 (char '\t' >> langCommandLineLn)
                  many blankLine
                  return $ SubInlineCmd lns

ifInlineCmd  = do char '|'
                  many $ oneOf "\t "
                  c <- langCommandLineNoProc
                  colonReturn
                  lns <- many1 (char '\t' >> langCommandLineLn)
                  many blankLine
                  return $ IfInlineCmd c lns

colonReturn = char ':' >> (many $ oneOf " \t") >> char '\n'

langFunc = do string "func "
              nm <- langWord
              args <- many langWord
              many $ oneOf " \t"
              colonReturn
              lns <- many1 (char '\t' >> langCommandLineLn)
              return $ Func nm (zip [1..] args) lns

langWord = try(langWordNoQuot) <|> try(langWordSQuot) <|> try(langWordDQuot)

langWordSQuot = do char '\''
                   w <- many (noneOf "\'")
                   char '\''
                   many $ oneOf " \t"
                   return $ ('\'':w) ++ "\'"


langWordDQuot = do char '"'
                   w <- many ( (try escDQuot) <|> (many1 $ noneOf "\"") )
                   char '"'
                   many $ oneOf " \t"
                   return $ ('\"':(concat w)) ++ "\"" 

langWordNoQuot = do w <- many1 (noneOf "\'\" :\n\t")
                    many $ oneOf " \t"
                    return w

langCommandLineLn = do ln <- langCommandLine
                       many blankLine
                       return ln

langCommandLine = try(heredoc) <|> try(langCommandLineOutfile)  
                               <|> try(langCommandLineOutstr) <|> try(langCommandLineNoProc)

-- file f = """ (here document)
heredoc = do string "file"
             many1 $ oneOf " \t"
             f <- langWord
             many $ oneOf " \t"
             char '='
             many $ oneOf " \t"
             string "\"\"\"\n"
             c <- manyTill heredocLine (try $ string "\t\"\"\"")
             return $ Heredoc (Write,f) (unlines c)

heredocLine = do tt <- string "\t\t"
                 str <- many $ noneOf "\n"
                 char '\n'
                 return str


-- file f = command args 
langCommandLineOutfile = do (string "file " >> (many $ char ' ') )
                            f <- langWord
                            ((many $ char ' ') >> (char '=') >> (many $ char ' ' ))
                            ln <- many1 langWord
                            return $ CommandLine [(Write,f)] ln

-- str f = command args 
langCommandLineOutstr  = do string "str "
                            many $ char ' '
                            f <- langWord
                            many $ char ' '
                            char '='
                            many $ char ' '
                            ln <- many1 langWord
                            return $ CommandLine [(Str,f)] ln

langCommandLineNoProc = many1 langWord >>= return . (CommandLine [])

blankLine = try(many (oneOf " \t") >> char '\n')
--blankLine = char '\n'
--
escDQuot = string "\\\""
