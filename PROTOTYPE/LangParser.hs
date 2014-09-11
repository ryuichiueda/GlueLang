module LangParser( parseGlueLang ) where

import Text.Parsec
import Text.Parsec.String
import LangStructure

parseGlueLang :: String -> Script
parseGlueLang str = case parse code "" str of
                          Right scr    -> scr 
                          Left err     -> Err ( show err )

code = do is <- many1 langImport
          many blankLine
          fs <- many1 (try(langFilter) <|> try(langIo))
          return $ Script is fs


langImport = do string "import "
                p <- langWord
                string "as "
                a <- langWord
                char '\n'
                return $ Import p a

langIo        = do string "io "
                   nm <- langWord
	           args <- many langWord
                   many $ oneOf " \t"
                   colonReturn
                   sb <- many1 (try(langIfBlock) <|> try(langSubBlock))
                   many $ char '\n'
                   many blankLine
                   return $ Io nm (zip [1..] args) sb

langSubBlock = do lns <- many1 (char '\t' >> langCommandLineLn)
                  many blankLine
                  return $ SubBlock lns

langIfBlock  = do char '|'
                  many $ oneOf "\t "
                  c <- langCommandLineNoIo
                  colonReturn
                  lns <- many1 (char '\t' >> langCommandLineLn)
                  many blankLine
                  return $ IfBlock c lns

colonReturn = char ':' >> (many $ oneOf " \t") >> char '\n'

langFilter = do string "filter "
                nm <- langWord
		args <- many langWord
                many $ oneOf " \t"
                colonReturn
                lns <- many1 (char '\t' >> langCommandLineLn)
                return $ Filter nm (zip [1..] args) lns

langWord = do w <- many1 (noneOf " :\n\t")
              many $ oneOf " \t"
              return w

langCommandLineLn = do ln <- langCommandLine
                       --char '\n'
                       many blankLine
                       --error $ show ln
                       return ln

langCommandLine = try(heredoc) <|> try(langCommandLineOutfile)  
                               <|> try(langCommandLineOutstr) <|> try(langCommandLineNoIo)

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

langCommandLineNoIo = many1 langWord >>= return . (CommandLine [])

blankLine = try(many (oneOf " \t") >> char '\n')
--blankLine = char '\n'
