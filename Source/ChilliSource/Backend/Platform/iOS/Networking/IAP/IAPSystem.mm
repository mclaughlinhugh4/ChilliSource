//
//  IAPSystem.cpp
//  Chilli Source
//
//  Created by Scott Downie on 12/06/2013.
//  Copyright (c) 2013 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Backend/Platform/iOS/Networking/IAP/IAPSystem.h>

#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/Cryptographic/BaseEncoding.h>
#include <ChilliSource/Core/String/StringUtils.h>

namespace ChilliSource
{
    namespace iOS
    {
        namespace
        {
            //---------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param List of product registrations
            /// @param Product Id to find
            ///
            /// @return Whether a product reg info exists in the list
            /// with the given Id.
            //---------------------------------------------------------------
            bool ContainsProductId(const std::vector<Networking::IAPSystem::ProductRegInfo>& in_productInfos, const std::string& in_productId)
            {
                for(u32 i=0; i<in_productInfos.size(); ++i)
                {
                    if(in_productInfos[i].m_id == in_productId)
                    {
                        return true;
                    }
                }
                
                return false;
            }
        }
        
        CS_DEFINE_NAMEDTYPE(IAPSystem);
        
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        bool IAPSystem::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return in_interfaceId == Networking::IAPSystem::InterfaceID || in_interfaceId == IAPSystem::InterfaceID;
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void IAPSystem::OnInit()
        {
            m_storeKitSystem = [[StoreKitIAPSystem alloc] init];
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void IAPSystem::RegisterProducts(const std::vector<ProductRegInfo>& in_productInfos)
        {
            CS_ASSERT(in_productInfos.empty() == false, "Must register at least one product");
            m_productRegInfos = in_productInfos;
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        std::string IAPSystem::GetProviderID() const
        {
            return "Apple";
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        std::string IAPSystem::GetProviderName() const
        {
            return "iTunes";
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        bool IAPSystem::IsPurchasingEnabled()
        {
            return [m_storeKitSystem isPurchasingEnabled] == YES;
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void IAPSystem::StartListeningForTransactionUpdates(const TransactionStatusDelegate& in_delegate)
        {
            CS_ASSERT(in_delegate != nullptr, "Cannot have null transaction delegate");
            m_transactionStatusDelegate = in_delegate;
            
            [m_storeKitSystem startListeningForTransactions:Core::MakeDelegate(this, &IAPSystem::OnTransactionUpdate)];
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void IAPSystem::OnTransactionUpdate(NSString* in_productID, StoreKitIAP::TransactionResult in_result, SKPaymentTransaction* in_skTransaction)
        {
            if(m_transactionStatusDelegate == nullptr)
                return;
            
            bool hasReceipt = false;
            Transaction::Status result = Transaction::Status::k_failed;
            switch(in_result)
            {
                case StoreKitIAP::TransactionResult::k_succeeded:
                    hasReceipt = true;
                    result = Transaction::Status::k_succeeded;
                    break;
                case StoreKitIAP::TransactionResult::k_failed:
                    result = Transaction::Status::k_failed;
                    break;
                case StoreKitIAP::TransactionResult::k_cancelled:
                    result = Transaction::Status::k_cancelled;
                    break;
                case StoreKitIAP::TransactionResult::k_restored:
                    hasReceipt = true;
                    result = Transaction::Status::k_restored;
                    break;
                case StoreKitIAP::TransactionResult::k_resumed:
                    hasReceipt = true;
                    result = Transaction::Status::k_resumed;
                    break;
            }
            
            TransactionSPtr transaction(new Transaction());
            transaction->m_productId = Core::StringUtils::NSStringToString(in_productID);
            transaction->m_transactionId = Core::StringUtils::NSStringToString(in_skTransaction.transactionIdentifier);
            if(hasReceipt)
            {
                transaction->m_receipt = Core::BaseEncoding::Base64Encode((s8*)[in_skTransaction.transactionReceipt bytes], [in_skTransaction.transactionReceipt length]);
            }
            
            m_transactionStatusDelegate(result, transaction);
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void IAPSystem::StopListeningForTransactionUpdates()
        {
            [m_storeKitSystem stopListeningForTransactions];
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void IAPSystem::RequestProductDescriptions(const std::vector<std::string>& in_productIds, const ProductDescDelegate& in_delegate)
        {
            CS_ASSERT(in_productIds.empty() == false, "Cannot request no product descriptions");
            CS_ASSERT(in_delegate != nullptr, "Cannot have null product description delegate");
            CS_ASSERT(m_productDescDelegate == nullptr, "Only 1 product description request can be active at a time");
            
            m_productDescDelegate = in_delegate;
            
            NSMutableSet* idSet = [[NSMutableSet alloc] initWithCapacity:in_productIds.size()];
			
			for (u32 i=0; i<in_productIds.size(); ++i)
            {
				[idSet addObject:Core::StringUtils::StringToNSString(in_productIds[i])];
			}
			
            [m_storeKitSystem requestProducts:idSet forDelegate:Core::MakeDelegate(this, &IAPSystem::OnProductDescriptionRequestComplete)];
            
            [idSet release];
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void IAPSystem::RequestAllProductDescriptions(const ProductDescDelegate& in_delegate)
        {
            std::vector<std::string> productIds;
            productIds.reserve(m_productRegInfos.size());
            
            for(u32 i=0; i<m_productRegInfos.size(); ++i)
            {
                productIds.push_back(m_productRegInfos[i].m_id);
            }
            
            RequestProductDescriptions(productIds, in_delegate);
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void IAPSystem::OnProductDescriptionRequestComplete(NSArray* in_products)
        {
            if(m_productDescDelegate == nullptr)
                return;
 
            std::vector<ProductDesc> descriptions;
            
            if(in_products != nil)
            {
                NSNumberFormatter* formatter = [[NSNumberFormatter alloc] init];
                [formatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
                [formatter setNumberStyle:NSNumberFormatterCurrencyStyle];
                
                for(SKProduct* product in in_products)
                {
                    ProductDesc description;
                    description.m_id = Core::StringUtils::NSStringToString(product.productIdentifier);
                    description.m_name = Core::StringUtils::NSStringToString(product.localizedTitle);
                    description.m_description = Core::StringUtils::NSStringToString(product.localizedDescription);
                    
                    [formatter setLocale:product.priceLocale];
                    description.m_formattedPrice = Core::StringUtils::NSStringToString([formatter stringFromNumber:product.price]);
                    
                    NSLocale* storeLocale = product.priceLocale;
                    description.m_countryCode = Core::StringUtils::NSStringToString((NSString*)CFLocaleGetValue((CFLocaleRef)storeLocale, kCFLocaleCountryCode));
                    
                    descriptions.push_back(description);
                }
                
                [formatter release];
            }

            m_productDescDelegate(descriptions);
            m_productDescDelegate = nullptr;
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void IAPSystem::CancelProductDescriptionsRequest()
        {
            [m_storeKitSystem cancelProductsRequest];
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void IAPSystem::RequestProductPurchase(const std::string& in_productId)
        {
            CS_ASSERT(ContainsProductId(m_productRegInfos, in_productId), "Products must be registered with the IAP system before purchasing");
            NSString* productID = Core::StringUtils::StringToNSString(in_productId);
            [m_storeKitSystem requestPurchaseWithProductID:productID andQuantity:1];
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void IAPSystem::CloseTransaction(const TransactionSPtr& in_transaction, const TransactionCloseDelegate& in_delegate)
        {
            CS_ASSERT(in_delegate != nullptr, "Cannot have null transaction close delegate");
            CS_ASSERT(m_transactionCloseDelegate == nullptr, "Only 1 transaction can be closed at a time");
            
            m_transactionCloseDelegate = in_delegate;
            
            [m_storeKitSystem closeTransactionWithID:Core::StringUtils::StringToNSString(in_transaction->m_transactionId)];
            
            m_transactionCloseDelegate(in_transaction->m_productId, in_transaction->m_transactionId, true);
            m_transactionCloseDelegate = nullptr;
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void IAPSystem::RestoreManagedPurchases()
        {
            [m_storeKitSystem restoreNonConsumablePurchases];
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void IAPSystem::OnDestroy()
        {
            [m_storeKitSystem release];
            m_productDescDelegate = nullptr;
            m_transactionStatusDelegate = nullptr;
            m_transactionCloseDelegate = nullptr;
            m_productRegInfos.clear();
            m_productRegInfos.shrink_to_fit();
        }
    }
}