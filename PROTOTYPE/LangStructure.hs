module LangStructure
( FilterName
, FilterArgs
, FilterCode
, Filter(..)
, Script(..)
) where

type FilterName = String
type FilterArgs = (Int,String)
type FilterCode = String
data Filter = Filter FilterName [FilterArgs] [FilterCode] deriving Show
data Script = Script [Filter] | Err String deriving Show
