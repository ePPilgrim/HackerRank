using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Moq;
using SimCorp.IMS.MiddleOffice.Engine.Api;
using SimCorp.IMS.MiddleOffice.Engine.Api.Common;
using SimCorp.IMS.MiddleOffice.Engine.Facade;
using SimCorp.IMS.MiddleOffice.Engine.Facade.Common;
using SimCorp.IMS.Framework.DataSharing.Service.Implementation;
using SimCorp.IMS.Framework.DataSharing;
using SimCorp.IMS.Framework.DataSharing.Queryable;

namespace SimCorp.IMS.MiddleOffice.Engine.Api.Test {
    [TestClass]
    public class DataSourceAdapterCalculationEngineTest {
        public List<Analytics> Analytics = new List<Analytics>();
        public List<CalculationArgument> CalculationArguments = new List<CalculationArgument>();
        public Mock<ICalculationEngineFacade> MockCalculationEngineFacade = new Mock<ICalculationEngineFacade>();
        public List<Results> Results = new List<Results>();

        public DataSourceAdapterCalculationEngineTest() {
            init();
            MockCalculationEngineFacade.Setup(x => x.GetSupportedAnalytics()).Returns(() => Analytics);
        }

        [TestMethod]
        public void TestMethod1() {
            var dataSource = new DataSourceAdapterCalculationEngine(MockCalculationEngineFacade.Object);
            dataSource.Initialize();

            var credentials = new OperationCredentials(Environment.UserName.ToUpper());

            var query1 = new QueryData() {
                EntityName = "Analytics",
                //PropertyPaths = new[] { "Id", "Key", "Currency.Name" },
               // Filter = "Currency.Name in (\"DKK\", \"NOK\") and Key != 0",
                MaxCount = 1000
            };
            OperationData result1 = dataSource.SelectAsync(credentials, query1).Result;

        }

        public void init() {
            foreach (var id in new[] {"IRR", "AnualIRR"}) {
                foreach (var formattype in new[] {FormatType.Numeric, FormatType.Amount}) {
                    foreach (var currencytypename in new[] {"QC", "RC", "PC"}) {
                        foreach (var periodname in new[] {"Year", "Month"}) {
                            Analytics.Add(new Analytics {
                                Id = id, Title = id, FormatType = formattype, TypeName = formattype.ToString(),
                                CurrencyTypeName = currencytypename, PeriodName = periodname
                            });
                        }
                    }
                }
            }

            var Date = new DateTime(2014, 12, 21);
            var tic = 1;
            foreach (var id in new[] {"IRR", "AnualIRR"}) {
                foreach (
                var groupingcriteria in new[] {GroupingCriteriaEnum.PortfolioKey, GroupingCriteriaEnum.SecurityKey}) {
                    foreach (var restrictionCriteria in
                    new[] {
                        RestrictionCriteriaEnum.AlternativeInvestmentAssetKey, RestrictionCriteriaEnum.PortfolioKey,
                        RestrictionCriteriaEnum.SecurityKey
                    }) {
                        foreach (var periodlength in new[] {1, -1}) {
                            var calcArg = new CalculationArgument {
                                AnalysisDates = new[] { Date.AddDays(1) },
                                Groupings = new[] {groupingcriteria},
                                AnalyticIds = new[] {id},
                                PeriodTermLength = tic*periodlength,
                                PeriodTermUnit = PeriodTermUnitEnum.Years,
                                RestrictionSet =
                                    new RestrictionSetEntity {
                                        Restrictions =
                                            new[] {
                                                new RestrictionEntity
                                                {Criteria = restrictionCriteria, Values = new[] {tic}}
                                            },
                                        OnlyIncludeActiveInvestment = true, OnlyIncludeSecuritiesLinkedToAnAsset = true
                                    }
                            };
                            if (calcArg.PeriodTermLength < 0) { calcArg.PeriodTermLength = null; }
                            if (tic%2 == 0) { calcArg.PeriodTermUnit = null; }
                            CalculationArguments.Add(calcArg);

                            var result =
                            new Results(new List<AnalyticsResult> {
                                new AnalyticsResult
                                {NodeId = tic.ToString(), AnalyticsId = id, CalculationDate = Date, Value = 3.14*tic}
                            });
                            var groupings = new List<GroupingCriteria>
                            {ApiEnumConverter<GroupingCriteriaEnum, GroupingCriteria>.Convert(groupingcriteria)};
                            var restrictionSet = calcArg.RestrictionSet.Convert();
                            if (calcArg.PeriodTermLength.HasValue && calcArg.PeriodTermUnit.HasValue) {
                                var perTermUnit =
                                ApiEnumConverter<PeriodTermUnitEnum, PeriodTermUnit>.Convert(
                                calcArg.PeriodTermUnit.Value);
                                MockCalculationEngineFacade.Setup(
                                x =>
                                x.Calculate(Date.Date, perTermUnit, calcArg.PeriodTermLength.Value,
                                calcArg.AnalyticIds.ToList(), restrictionSet, groupings)).Returns(() => result);
                            }
                            else {
                                MockCalculationEngineFacade.Setup(
                                x => x.Calculate(Date.Date, calcArg.AnalyticIds.ToList(), restrictionSet, groupings))
                                .Returns(() => result);
                            }
                        }
                    }
                }
            }
        }
    }
}