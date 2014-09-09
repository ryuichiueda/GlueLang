module ToBash
( toBash
) where

import LangStructure
import qualified Data.Text as D

toBash :: Script -> String
toBash (Script _ fs) = unlines (header:(map filterToFunc fs) ++ [footer])
    where header = "#!/bin/bash -evx\n"
          footer = mainArgs fs

mainArgs :: [Block] -> String
mainArgs ((Filter "main" args _):fs) = unwords ("main" :opts)
    where opts = [ "\"" ++ ('$':(show n)) ++ "\"" | n <- [1..(length args)]]
mainArgs _ = ""

filterToFunc :: Block -> String
filterToFunc (Filter fname opts codes) = "function " ++ fname ++ "(){\n"
                                         ++ "\t" ++ (pipeCon $ map (convArgs opts) codes) 
                                         ++ "}\n"

pipeCon :: [String] -> String
pipeCon [s] = s ++ "\n"
pipeCon (s:ss) = s ++ " | " ++ pipeCon ss

convArgs :: [Args] -> Code -> String
convArgs [] str = str
convArgs ((n,op):ops) str = convArgs ops $ D.unpack (D.replace (D.pack op) (D.pack $ ('$':show n)) (D.pack str))
