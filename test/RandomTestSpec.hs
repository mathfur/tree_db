{-# LANGUAGE ForeignFunctionInterface #-}
{-# LANGUAGE OverloadedStrings #-}

module RandomTestSpec (spec) where

import Test.Hspec
import Test.Hspec.HUnit
import Test.Hspec.QuickCheck
import Test.QuickCheck
import Test.QuickCheck.Property as P
import Test.QuickCheck.Arbitrary
import Debug.Trace
import Foreign
import Foreign.C
import Foreign.C.Types
import Foreign.Ptr
import Criterion.Main
import Control.Applicative
import qualified Data.ByteString as B
import qualified Data.ByteString.Char8 as C8

data CVoid
newtype HashTable = MkHashTable (Ptr CVoid) -- TODO: CVoidの部分はこれに変える

instance Arbitrary B.ByteString where
    arbitrary = do
      r <- listOf1 (choose ('a', 'z'))
      return $ C8.pack r   -- r :: String

data GetDescendantsArgument = GetDescendantsArgument {
                                centerArgs :: B.ByteString, 
                                depthArgs :: Int,
                                edgesArgs :: [(B.ByteString, B.ByteString)]
                              } deriving (Show, Eq)

instance Arbitrary GetDescendantsArgument where
    arbitrary = do
      nodes <- listOf1 arbitrary
      center_idx <- arbitrary
      depth <- abs <$> arbitrary
      edge_idx_pairs <- arbitrary
      let edges = map (\(i, j) -> (id2label nodes i, id2label nodes j)) edge_idx_pairs
      return $ GetDescendantsArgument (id2label nodes center_idx) depth edges

id2label :: [a] -> Int -> a
id2label nodes idx = nodes !! (idx `mod` (length nodes))

get_result :: GetDescendantsArgument -> IO Int
get_result args = do
      tbl <- init_table 1000

      mapM_ (\(start,end) -> wrap_add_edge_to_table tbl start end) $ edgesArgs args
      r <- wrap_get_descendants tbl (centerArgs args) (depthArgs args)
      
      free_table tbl
      return r

-- テスト1: depthが0なら結果数も常に0個
prop_return_value_is_zero_when_depth_is_zero :: GetDescendantsArgument -> Property
prop_return_value_is_zero_when_depth_is_zero args = (0 < (length $ edgesArgs args)) ==> morallyDubiousIOProperty $ do
      r <- get_result (args { depthArgs = 0 })
      return $ (0 == r)

-- テスト2: depthを1増やすと結果数は減らない
prop_2 :: (GetDescendantsArgument, Int, Int) -> Property
prop_2 (args,depth1,depth2) = (0 < (length $ edgesArgs args) && 0 < depth1 && depth1 < depth2) ==> morallyDubiousIOProperty $ do
      r1 <- get_result (args { depthArgs = depth1 })
      r2 <- get_result (args { depthArgs = depth2 })
      return $ r1 <= r2

wrap_get_descendants :: Ptr CVoid -> B.ByteString -> Int -> IO Int
wrap_get_descendants tbl center_name depth = do
    let x :: B.ByteString
        x = "xxxxxxxxxxxxxxxxxxxxxxx" -- TODO: 書き直す
    r <- B.useAsCStringLen x $
           \(cstr, len) -> B.useAsCString center_name $
             \c_center -> get_descendants tbl c_center (fromIntegral depth) cstr 6
    return r

wrap_add_edge_to_table :: Ptr CVoid -> B.ByteString -> B.ByteString -> IO ()
wrap_add_edge_to_table tbl start end = do
   r <- B.useAsCString start $
          \c_start -> B.useAsCString end $
            \c_end -> add_edge_to_table tbl c_start c_end
   return ()

spec :: Spec
spec = do
  describe "#get_descendants" $ do
    prop "depth == zero => return value == 0" prop_return_value_is_zero_when_depth_is_zero
    prop "prop 2" prop_2

foreign import ccall init_table :: CInt -> IO (Ptr CVoid)
foreign import ccall free_table :: Ptr CVoid -> IO ()
foreign import ccall add_edge_to_table :: Ptr CVoid -> CString -> CString -> IO Int
foreign import ccall get_descendants :: Ptr CVoid -> CString -> CInt -> Ptr CChar -> Int -> IO Int
